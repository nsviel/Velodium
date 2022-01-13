#include "CT_ICP.h"

#include "CT_ICP/SLAM_optim_ceres.h"
#include "CT_ICP/SLAM_optim_gn.h"

#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"
#include "../../Engine/Scene.h"
#include "../../Operation/Transformation/Transforms.h"


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
  this->verbose = false;
  this->frame_all = true;
  this->slamMap_voxelized = false;

  this->thres_ego_trans = 2.0f;
  this->thres_ego_rotat = 15;
  this->thres_pose_trans = 3.0f;
  this->thres_pose_rotat = 15.0f;
  this->thres_optimMinNorm = 0.1;

  this->min_subset_distance = 5.0f;
  this->max_subset_distance = 100.0f;
  this->max_voxel_distance = 150.0f;
  this->map_max_voxelNbPoints = 20;
  this->min_voxel_distance = 0.05;
  this->frame_max = 0;
  this->frame_ID = 0;
  this->map_size_old = 0;
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

    this->add_pointsToSlamMap(subset);
    this->add_pointsToLocalMap(frame);

    this->end_updateSubsetLocation(subset);
    this->end_clearTooFarVoxels(frame->trans_e);

    //--------------
    float duration = toc();
    this->end_statistics(duration, frame, frame_m1, subset);
  }

  this->end_slamVoxelization(cloud);

  //---------------------------
  delete map;
  delete map_cloud;
}
void CT_ICP::compute_slam_online(Cloud* cloud, int i){
  tic();
  //---------------------------

  Subset* subset = &cloud->subset[i];
  Frame* frame = &cloud->subset[i].frame;
  Frame* frame_m1 = &cloud->subset[i-1].frame;
  Frame* frame_m2 = &cloud->subset[i-2].frame;
  frame->ID = i;

  this->init_frameTimestamp(subset);
  this->init_frameChain(frame, frame_m1, frame_m2);
  this->init_distortion(frame);

  this->compute_gridSampling(subset);
  this->compute_optimization(frame, frame_m1);
  this->compute_assessRegistration(frame, frame_m1);

  this->add_pointsToLocalMap(frame);

  this->end_updateSubsetLocation(subset);
  this->end_clearTooFarVoxels(frame->trans_e);

  //---------------------------
  float duration = toc();
  this->end_statistics(duration, frame, frame_m1, subset);
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
  if(isnan(norm)){
    norm = 0;
  }
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

  //Test 1: check ego distance
  float ego_trans = (frame->trans_e - frame->trans_b).norm();
  if(ego_trans > thres_ego_trans){
    cout<<"[error] Ego translation too important ";
    cout<<"["<<ego_trans<<"/"<<thres_ego_trans<<"]"<<endl;
    sucess = false;
  }

  //Test 2: Ego angular distance
  float ego_rotat = AngularDistance(frame->rotat_b, frame->rotat_e);
  if(ego_rotat > thres_ego_rotat){
    cout<<"[error] Ego rotation too important ";
    cout<<"["<<ego_rotat<<"/"<<thres_ego_rotat<<"]"<<endl;
    sucess = false;
  }

  //Test 3: check relative distance and orientation between two poses
  if(frame->ID > 2){
    float diff_trans = (frame->trans_b - frame_m1->trans_b).norm() + (frame->trans_e - frame_m1->trans_e).norm();
    float diff_rotat = AngularDistance(frame_m1->rotat_b, frame->rotat_b) + AngularDistance(frame_m1->rotat_e, frame->rotat_e);
    if(diff_trans > thres_pose_trans){
      cout<<"[error] Pose translation too important ";
      cout<<"["<<diff_trans<<"/"<<thres_pose_trans<<"]"<<endl;
      sucess = false;
    }
    if(diff_rotat > thres_pose_rotat){
      cout<<"[error] Pose rotation too important ";
      cout<<"["<<diff_rotat<<"/"<<thres_pose_rotat<<"]"<<endl;
      sucess = false;
    }
  }

  //Test 4: check if ICP has converged
  float Xscore = gnManager->get_Xscore();
  if(Xscore > thres_optimMinNorm){
    cout<<"[error] Optimization score too important ";
    cout<<"["<<Xscore<<"/"<<thres_optimMinNorm<<"]"<<endl;
    sucess = false;
  }

  //If unsucess, reinitialize transformations
  if(sucess == false){
    frame->rotat_b = frame_m1->rotat_b;
    frame->trans_b = frame_m1->trans_b;
    frame->rotat_e = frame_m1->rotat_b;
    frame->trans_e = frame_m1->trans_b;
  }

  //---------------------------
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

void CT_ICP::end_updateSubsetLocation(Subset* subset){
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
void CT_ICP::end_statistics(float duration, Frame* frame, Frame* frame_m1, Subset* subset){
  //---------------------------

  //Fill stats
  frame->time_slam = duration;
  frame->map_size_abs = map->size();
  frame->map_size_rlt = map->size() - map_size_old;
  this->map_size_old = map->size();

  //Transformation parameters
  vec3 trans_abs = vec3(frame->trans_b(0), frame->trans_b(1), frame->trans_b(2));
  vec3 trans_rlt;
  trans_rlt.x = frame->trans_b(0) - frame_m1->trans_b(0);
  trans_rlt.y = frame->trans_b(1) - frame_m1->trans_b(1);
  trans_rlt.z = frame->trans_b(2) - frame_m1->trans_b(2);

  Transforms transformManager;
  vec3 rotat_abs = transformManager.compute_anglesFromTransformationMatrix(frame->rotat_b);
  vec3 f0_rotat = transformManager.compute_anglesFromTransformationMatrix(frame->rotat_b);
  vec3 f1_rotat = transformManager.compute_anglesFromTransformationMatrix(frame_m1->rotat_b);

  vec3 rotat_rlt;
  rotat_rlt.x = f0_rotat.x - f0_rotat.x;
  rotat_rlt.y = f0_rotat.y - f1_rotat.y;
  rotat_rlt.z = f0_rotat.z - f1_rotat.z;

  frame->trans_abs = trans_abs;
  frame->rotat_abs = rotat_abs;
  frame->trans_rlt = trans_rlt;
  frame->rotat_rlt = rotat_rlt;

  //Terminal result
  if(verbose){
    cout<<"[sucess] SLAM - "<<subset->name.c_str();
    cout<<" "<<to_string(frame->ID)<<"/"<< frame_max;
    cout<< " [" <<duration<< " ms]"<<endl;
  }

  //Consol result
  string result = "SLAM " + subset->name + " - " + to_string(frame->ID) + " [" + to_string((int)duration) + " ms]";
  console.AddLog("#", result);

  //---------------------------
}
