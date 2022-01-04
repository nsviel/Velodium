#include "CT_ICP.h"

#include "CT_ICP/SLAM_optim_ceres.h"
#include "CT_ICP/SLAM_optim_gn.h"

#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"

#include "../../Engine/Scene.h"


//Constructor / Destructor
CT_ICP::CT_ICP(){
  //---------------------------

  this->sceneManager = new Scene();
  this->normalManager = new SLAM_normal();
  this->ceresManager = new SLAM_optim_ceres(normalManager);
  this->gnManager = new SLAM_optim_gn(normalManager);
  this->map = new voxelMap();
  this->map_cloud = new slamMap();

  this->solver_ceres = false;
  this->solver_GN = true;
  this->verbose = true;
  this->frame_all = true;
  this->slamMap_voxelized = false;

  this->thres_min_distance = 5.0f;
  this->thres_trans_norm = 0.005;
  this->thres_rotat_norm = 0.05;

  this->min_subset_distance = 5.0f;
  this->max_subset_distance = 100.0f;
  this->max_voxel_distance = 150.0f;
  this->map_max_voxelNbPoints = 20;
  this->min_voxel_distance = 0.05;
  this->frame_max = 0;
  this->frame_ID = 0;
  this->nb_thread = 8;

  this->voxel_size_gridMap = 1;
  this->voxel_size_localMap = 1;
  this->voxel_size_slamMap = 0.5;

  //---------------------------
}
CT_ICP::~CT_ICP(){}

//Main functions
void CT_ICP::compute_slam(Cloud* cloud){
  map = new voxelMap();
  map_cloud = new slamMap();

  if(cloud == nullptr) return;
  if(frame_all) frame_max = cloud->nb_subset;
  //---------------------------

  for(int i=0; i<frame_max; i++){
    Subset* subset = &cloud->subset[i];
    Frame* frame = &cloud->subset[i].frame;
    Frame* frame_m1 = &cloud->subset[i-1].frame;
    Frame* frame_m2 = &cloud->subset[i-2].frame;
    frame->ID = i;
    tic();
    //--------------

    this->init_frameTimestamp(subset);
    this->init_frameChain(frame, frame_m1, frame_m2);
    this->init_distortion(frame);

    this->compute_gridSampling(subset);
    this->compute_optimization(frame, frame_m1);
    this->compute_assessRegistration(frame, frame_m1);

    this->add_pointsToSubset(subset);
    this->add_pointsToSlamMap(subset);
    this->add_pointsToLocalMap(frame);

    //--------------
    float duration = toc();
    this->end_time(duration, frame, subset);
    this->end_clearTooFarVoxels(frame->trans_e);
  }

  this->end_slamVoxelization(cloud);

  //---------------------------
  delete map;
  delete map_cloud;
}
void CT_ICP::compute_slam_online(Cloud* cloud){
  int last_subset = cloud->subset.size() - 1;
  Subset* subset = &cloud->subset[last_subset];
  Frame* frame = &cloud->subset[last_subset].frame;
  Frame* frame_m1 = &cloud->subset[last_subset-1].frame;
  Frame* frame_m2 = &cloud->subset[last_subset-2].frame;
  frame->ID = last_subset;
  frame_max = cloud->subset.size();
  tic();
  //---------------------------

  this->init_frameTimestamp(subset);
  this->init_frameChain(frame, frame_m1, frame_m2);
  this->init_distortion(frame);

  this->compute_gridSampling(subset);
  this->compute_optimization(frame, frame_m1);

  this->add_pointsToSubset(subset);
  this->add_pointsToSlamMap(subset);
  this->add_pointsToLocalMap(frame);

  float duration = toc();
  this->end_time(duration, frame, subset);

  //---------------------------
}

//Support functions
void CT_ICP::reset(){
  //---------------------------

  this->frame_ID = 0;

  delete map;
  delete map_cloud;

  this->map = new voxelMap();
  this->map_cloud = new slamMap();

  //---------------------------
}
void CT_ICP::set_nb_thread(int value){
  //---------------------------

  this->nb_thread = value;
  normalManager->set_nb_thread(nb_thread);
  gnManager->set_nb_thread(nb_thread);

  //---------------------------
}
float CT_ICP::AngularDistance(Eigen::Matrix3d &rota, Eigen::Matrix3d &rotb){
  float norm = ((rota * rotb.transpose()).trace() - 1) / 2;
  norm = std::acos(norm) * 180 / M_PI;
  return norm;
}

//SLAM sub-functions
void CT_ICP::init_frameTimestamp(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Timestamp
  vector<float>& ts = subset->ts;
  if(ts.size() != 0 && frame->ID > 1){

    //Retrieve min & max
    double min = ts[0];
    double max = ts[0];
    for(int i=0; i<ts.size(); i++){
      if(ts[i] > max) max = ts[i];
      if(ts[i] < min) min = ts[i];
    }
    subset->ts_b = min;
    subset->ts_e = max;

    //Normalization
    subset->ts_n.clear();
    for(int i=0; i<ts.size(); i++){
      double ts_n = (ts[i] - min) / (max - min);
      subset->ts_n.push_back(ts_n);
    }

  }else{
    subset->ts_n = fct_ones(subset->xyz.size());
    subset->ts_b = 1.0f;
    subset->ts_e = 1.0f;
  }

  //---------------------------
}
void CT_ICP::init_frameChain(Frame* frame, Frame* frame_m1, Frame* frame_m2){
  //---------------------------

  //i == 0 is the reference frame
  if(frame->ID < 2){
    frame->rotat_b = Eigen::Matrix3d::Identity();
    frame->rotat_e = Eigen::Matrix3d::Identity();
    frame->trans_b = Eigen::Vector3d::Zero();
    frame->trans_e = Eigen::Vector3d::Zero();
  }
  //Second frame
  else if(frame->ID == 2){
    // Different for the second frame due to the bootstrapped elasticity
    Eigen::Matrix3d rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3d trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    frame->rotat_b = frame_m1->rotat_e;
    frame->trans_b = frame_m1->trans_e;
    frame->rotat_e = rotat_next_e;
    frame->trans_e = trans_next_e;
  }
  //Other frame
  else if(frame->ID > 2){
    // When continuous: use the previous begin_pose as reference
    Eigen::Matrix3d rotat_next_b = frame_m1->rotat_b * frame_m2->rotat_b.inverse() * frame_m1->rotat_b;
    Eigen::Vector3d trans_next_b = frame_m1->trans_b + frame_m1->rotat_b * frame_m2->rotat_b.inverse() * (frame_m1->trans_b - frame_m2->trans_b);
    Eigen::Matrix3d rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3d trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    frame->rotat_b = rotat_next_b;
    frame->trans_b = trans_next_b;
    frame->rotat_e = rotat_next_e;
    frame->trans_e = trans_next_e;
  }

  //Essai sans option
  if(false && frame->ID >= 2){
    frame->rotat_b = frame_m1->rotat_b;
    frame->trans_b = frame_m1->trans_b;
    frame->rotat_e = frame_m1->rotat_b;
    frame->trans_e = frame_m1->trans_b;
  }

  //---------------------------
}
void CT_ICP::init_distortion(Frame* frame){
  //---------------------------

  if(frame->ID > 1){
    Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
    Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
    Eigen::Vector3d trans_b = frame->trans_b;
    Eigen::Vector3d trans_e = frame->trans_e;

    // Distorts the frame (put all raw_points in the coordinate frame of the pose at the end of the acquisition)
    Eigen::Quaterniond quat_e_inv = quat_e.inverse(); // Rotation of the inverse pose
    Eigen::Vector3d trans_e_inv = -1.0 * (quat_e_inv * trans_e); // Translation of the inverse pose

    for (int i=0; i < frame->xyz.size(); i++) {

      float ts_n = frame->ts_n[i];
      Eigen::Vector3d& point = frame->xyz_raw[i];

      Eigen::Quaterniond quat_n = quat_b.slerp(ts_n, quat_e).normalized();
      Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

      // Distort Raw Keypoints
      point = quat_e_inv * (quat_n * point + t) + trans_e_inv;
    }
  }

  //---------------------------
}

void CT_ICP::compute_gridSampling(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  vector<Eigen::Vector3d>& frame_xyz = frame->xyz;
  vector<Eigen::Vector3d>& frame_raw = frame->xyz_raw;
  vector<float>& frame_ts_n = frame->ts_n;

  //Subsample the scan with voxels
  gridMap grid;
  for (int j=0; j<subset->xyz.size(); j++){
    Eigen::Vector3d xyz = glm_to_eigen_vec3_d(subset->xyz[j]);
    float ts_n = subset->ts_n[j];

    int kx = static_cast<int>(xyz(0) / voxel_size_gridMap);
    int ky = static_cast<int>(xyz(1) / voxel_size_gridMap);
    int kz = static_cast<int>(xyz(2) / voxel_size_gridMap);
    string voxel_id = to_string(kx) + " " + to_string(ky) + " " + to_string(kz);

    Eigen::Vector4d point(xyz(0), xyz(1), xyz(2), ts_n);
    grid[voxel_id].push_back(point);
  }

  //Clear vectors
  frame_xyz.clear();
  frame_raw.clear();
  frame_ts_n.clear();

  //Take one point inside each voxel
  gridMap::iterator it;
  for(auto it = grid.begin(); it != grid.end(); ++it) {
    if (it->second.size() > 0) {
      Eigen::Vector4d point = it->second[0];
      Eigen::Vector3d xyz(point(0), point(1), point(2));
      float ts_n = point(3);

      frame_xyz.push_back(xyz);
      frame_ts_n.push_back(ts_n);
    }
  }
  frame_raw = frame_xyz;

  //---------------------------
}
void CT_ICP::compute_optimization(Frame* frame, Frame* frame_m1){
  //---------------------------

  if(frame->ID >= 1){
    if(solver_GN){
      gnManager->optim_GN(frame, frame_m1, map);
    }else if(solver_ceres){
      ceresManager->optim_test(frame, frame_m1, map);
    }
  }

  //---------------------------
}
void CT_ICP::compute_assessRegistration(Frame* frame, Frame* frame_m1){
  bool sucess = true;
  //---------------------------

  float relative_distance = (frame->trans_e - frame->trans_b).norm();
  if(relative_distance > thres_min_distance){
    sucess = false;
  }

  float diff_trans = (frame_m1->trans_b - frame->trans_b).norm() + (frame_m1->trans_e - frame->trans_e).norm();
  float diff_rotat = AngularDistance(frame_m1->rotat_b, frame->rotat_b) + AngularDistance(frame_m1->rotat_e, frame->rotat_e);
  if(diff_trans > thres_trans_norm){
    sucess = false;
  }
  if(diff_rotat > thres_rotat_norm){
    sucess = false;
  }



  /*bool success = summary.success;
  if (summary.robust_level == 0
    && (summary.relative_orientation > options_.robust_threshold_relative_orientation
    || summary.ego_orientation > options_.robust_threshold_ego_orientation)) {
      if (summary.robust_level < options_.robust_num_attempts_when_rotation) {
          summary.error_message = "Large rotations require at a robust_level of at least 1 (got:" +
                                  std::to_string(summary.robust_level) + ").";
          return false;
      }
  }

  if (summary.relative_distance > options_.robust_relative_trans_threshold) {
      summary.error_message = "The relative distance is too important";
      return false;
  }

  // Only do neighbor assessment if enough motion
  bool do_neighbor_assessment = summary.distance_correction > 0.1;
  do_neighbor_assessment |= summary.relative_distance > options_.robust_neighborhood_min_dist;
  do_neighbor_assessment |= summary.relative_orientation > options_.robust_neighborhood_min_orientation;

  if (do_neighbor_assessment && registered_frames_ > options_.init_num_frames) {
      if (options_.robust_registration) {
          const double kSizeVoxelMap = options_.ct_icp_options.size_voxel_map;
          Voxel voxel;
          double ratio_empty_voxel = 0;
          double ratio_half_full_voxel = 0;

          for (auto &point: points) {
              voxel = Voxel::Coordinates(point.pt, kSizeVoxelMap);
              if (voxel_map_.find(voxel) == voxel_map_.end())
                  ratio_empty_voxel += 1;
              if (voxel_map_.find(voxel) != voxel_map_.end() &&
                  voxel_map_.at(voxel).NumPoints() > options_.max_num_points_in_voxel / 2) {
                  // Only count voxels which have at least
                  ratio_half_full_voxel += 1;
              }
          }

          ratio_empty_voxel /= points.size();
          ratio_half_full_voxel /= points.size();

          if (log_stream != nullptr)
              *log_stream << "[Quality Assessment] Keypoint Ratio of voxel half occupied: " <<
                          ratio_half_full_voxel << std::endl
                          << "[Quality Assessment] Keypoint Ratio of empty voxel " <<
                          ratio_empty_voxel << std::endl;
          if (ratio_half_full_voxel < options_.robust_full_voxel_threshold ||
              ratio_empty_voxel > options_.robust_empty_voxel_threshold) {
              success = false;
              if (ratio_empty_voxel > options_.robust_empty_voxel_threshold)
                  summary.error_message = "[Odometry::AssessRegistration] Ratio of empty voxels " +
                                          std::to_string(ratio_empty_voxel) + "above threshold.";
              else
                  summary.error_message = "[Odometry::AssessRegistration] Ratio of half full voxels " +
                                          std::to_string(ratio_half_full_voxel) + "below threshold.";

          }
      }
  }

  if (summary.relative_distance > options_.distance_error_threshold) {
      if (log_stream != nullptr)
          *log_stream << "Error in ego-motion distance !" << std::endl;
      return false;
  }




  auto increase_robustness_level = [&]() {
    previous_frame = summary.frame;
    // Handle the failure cases
    trajectory_[index_frame] = initial_estimate;
    ct_icp_options.voxel_neighborhood = std::min(++ct_icp_options.voxel_neighborhood,
                                                 options_.robust_max_voxel_neighborhood);
    ct_icp_options.ls_max_num_iters += 30;
    if (ct_icp_options.max_num_residuals > 0)
        ct_icp_options.max_num_residuals = ct_icp_options.max_num_residuals * 2;
    ct_icp_options.num_iters_icp = min(ct_icp_options.num_iters_icp + 20, 50);
    ct_icp_options.threshold_orientation_norm = max(
            ct_icp_options.threshold_orientation_norm / 10, 1.e-5);
    ct_icp_options.threshold_translation_norm = max(
            ct_icp_options.threshold_orientation_norm / 10, 1.e-4);
    sample_voxel_size = std::max(sample_voxel_size / 1.5, min_voxel_size);

    ct_icp_options.ls_sigma *= 1.2;
    ct_icp_options.max_dist_to_plane_ct_icp *= 1.5;
};*/

  //---------------------------
}

void CT_ICP::add_pointsToSubset(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  //Update frame root
  Eigen::Vector3d root = glm_to_eigen_vec3_d(subset->root);
  Eigen::Matrix3d R = quat_b.toRotationMatrix();
  Eigen::Vector3d t = trans_b;
  root = R * root + t;
  subset->root = eigen_to_glm_vec3_d(root);

  //Update subset position
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<subset->xyz.size(); i++){
    Eigen::Vector3d point = glm_to_eigen_vec3_d(subset->xyz[i]);
    float ts_n = subset->ts_n[i];

    Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;
    point = R * point + t;

    subset->xyz[i] = eigen_to_glm_vec3_d(point);
  }

  //---------------------------
  sceneManager->update_subset_location(subset);
  sceneManager->update_subset_color(subset);
}
void CT_ICP::add_pointsToSlamMap(Subset* subset){
  //---------------------------

  //Insert points into cloud global slam map
  if(slamMap_voxelized){

    for (int i=0; i<subset->xyz.size(); i++){
      vec3 point = subset->xyz[i];

      int kx = static_cast<int>(point.x / voxel_size_slamMap);
      int ky = static_cast<int>(point.y / voxel_size_slamMap);
      int kz = static_cast<int>(point.z / voxel_size_slamMap);
      string voxel_id = to_string(kx) + " " + to_string(ky) + " " + to_string(kz);

      //if the voxel already exists
      if(map_cloud->find(voxel_id) != map_cloud->end()){
        //Get corresponding voxel
        vector<vec3>& voxel_xyz = map_cloud->find(voxel_id).value();

        //If the voxel is not full
        if (voxel_xyz.size() < map_max_voxelNbPoints){
          voxel_xyz.push_back(point);
          subset->xyz_voxel.push_back(point);
        }
      }
      //else create it
      else{
        vector<vec3> vec;

        vec.push_back(point);
        subset->xyz_voxel.push_back(point);

        map_cloud->insert({voxel_id, vec});
      }
    }
  }

  //---------------------------
}
void CT_ICP::add_pointsToLocalMap(Frame* frame){
  //---------------------------

  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d point = frame->xyz[i];
    float dist = fct_distance_origin(point);

    if(dist > min_subset_distance && dist < max_subset_distance){
      int vx = static_cast<int>(point(0) / voxel_size_localMap);
      int vy = static_cast<int>(point(1) / voxel_size_localMap);
      int vz = static_cast<int>(point(2) / voxel_size_localMap);

      //Search for pre-existing voxel in local map
      string voxel_id = to_string(vx) + " " + to_string(vy) + " " + to_string(vz);

      //if the voxel already exists
      if(map->find(voxel_id) != map->end()){
        //Get corresponding voxel
        vector<Eigen::Vector3d>& voxel_xyz = map->find(voxel_id).value();

        //If the voxel is not full
        if (voxel_xyz.size() < map_max_voxelNbPoints){

          //Check if minimal distance with voxel points is respected
          float dist_min = 10000;
          for(int j=0; j<voxel_xyz.size(); j++){
            Eigen::Vector3d& voxel_point = voxel_xyz[j];
            float dist = fct_distance(point, voxel_point);
            if (dist < dist_min) {
              dist_min = dist;
            }
          }

          //If all conditions are fullfiled, add the point to local map
          if (dist_min > min_voxel_distance) {
            voxel_xyz.push_back(point);
          }

        }
      }
      //else create it
      else{
        vector<Eigen::Vector3d> vec;
        vec.push_back(point);
        map->insert({voxel_id, vec});
      }
    }
  }

  //---------------------------
}

void CT_ICP::end_clearTooFarVoxels(Eigen::Vector3d &current_location){
  vector<string> voxels_to_erase;
  //---------------------------

  for(auto it = map->begin(); it != map->end(); ++it){
    Eigen::Vector3d voxel_point = it->second[0];
    float dist = fct_distance(voxel_point, current_location);

    if(dist > max_voxel_distance){
      voxels_to_erase.push_back(it->first);
    }

  }

  for(int i=0; i<voxels_to_erase.size(); i++){
    map->erase(voxels_to_erase[i]);
  }

  //---------------------------
}
void CT_ICP::end_slamVoxelization(Cloud* cloud){
  //---------------------------

  if(slamMap_voxelized){
    for(int i=0; i<frame_max; i++){
      Subset* subset = &cloud->subset[i];
      subset->xyz = subset->xyz_voxel;
    }
  }

  //---------------------------
}
void CT_ICP::end_time(float duration, Frame* frame, Subset* subset){
  //---------------------------

  frame->time_slam = duration;
  if(verbose){
    cout<<"[sucess] SLAM - "<<subset->name.c_str();
    cout<<" "<<to_string(frame->ID)<<"/"<< frame_max;
    cout<< " [" <<duration<< " ms]"<<endl;
  }

  //---------------------------
}
