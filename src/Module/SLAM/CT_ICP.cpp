#include "CT_ICP.h"

#include "CT_ICP/SLAM_optim_ceres.h"
#include "CT_ICP/SLAM_optim_gn.h"
#include "CT_ICP/SLAM_assessment.h"
#include "CT_ICP/SLAM_localMap.h"

#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"
#include "../../Engine/Scene.h"
#include "../../Engine/Glyphs.h"
#include "../../Operation/Transformation/Transforms.h"


//Constructor / Destructor
CT_ICP::CT_ICP(){
  //---------------------------

  this->sceneManager = new Scene();
  this->glyphManager = new Glyphs();
  this->normalManager = new SLAM_normal();
  this->ceresManager = new SLAM_optim_ceres(normalManager);
  this->gnManager = new SLAM_optim_gn(normalManager);
  this->assessManager = new SLAM_assessment(gnManager);
  this->mapManager = new SLAM_localMap();

  this->solver_ceres = false;
  this->solver_GN = true;
  this->verbose = false;
  this->frame_all = true;

  this->frame_max = 0;
  this->map_frame_ID = 0;
  this->map_frame_begin_ID = 0;
  this->map_size_old = 0;
  this->nb_thread = 8;

  //---------------------------
}
CT_ICP::~CT_ICP(){}

//Main functions
void CT_ICP::compute_slam(Cloud* cloud){
  mapManager->reset();
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

    mapManager->compute_gridSampling(subset);

    this->compute_optimization(frame, frame_m1);
    this->compute_assessment(cloud, i);

    mapManager->add_pointsToSlamMap(subset);
    mapManager->add_pointsToLocalMap(frame);
    mapManager->end_clearTooFarVoxels(frame->trans_e);

    this->compute_updateLocation(subset);

    //--------------
    float duration = toc();
    this->compute_statistics(duration, frame, frame_m1, subset);
  }

  mapManager->end_slamVoxelization(cloud, frame_max);

  //---------------------------
}
void CT_ICP::compute_slam_online(Cloud* cloud, int i){
  Subset* subset = &cloud->subset[i];
  Frame* frame = &subset->frame;

  if(frame->is_slamed == false && i >= map_frame_begin_ID){
    tic();
    //---------------------------

    Frame* frame_m1 = &cloud->subset[i-1].frame;
    Frame* frame_m2 = &cloud->subset[i-2].frame;

    this->init_frameID(frame, i);
    this->init_frameTimestamp(subset);
    this->init_frameChain(frame, frame_m1, frame_m2);
    this->init_distortion(frame);

    mapManager->compute_gridSampling(subset);

    this->compute_optimization(frame, frame_m1);
    this->compute_assessment(cloud, i);

    mapManager->add_pointsToLocalMap(frame);
    mapManager->end_clearTooFarVoxels(frame->trans_e);

    this->compute_updateLocation(subset);

    //---------------------------
    float duration = toc();
    this->compute_statistics(duration, frame, frame_m1, subset);
    glyphManager->update(subset);
  }
}

//SLAM sub-functions
void CT_ICP::init_frameID(Frame* frame, int i){
  //---------------------------

  if(map_frame_ID == 0){
    map_frame_begin_ID = i;
  }

  frame->ID = map_frame_ID;

  map_frame_ID++;

  //---------------------------
}
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

void CT_ICP::compute_optimization(Frame* frame, Frame* frame_m1){
  voxelMap* map = mapManager->get_localmap();
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
void CT_ICP::compute_assessment(Cloud* cloud, int i){
  Frame* frame = &cloud->subset[i].frame;
  Frame* frame_m1 = &cloud->subset[i-1].frame;
  bool sucess = true;
  //---------------------------

  sucess = assessManager->compute_assessment_abs(frame, frame_m1);
  sucess = assessManager->compute_assessment_rlt(cloud, i);

  //If unsucess, reinitialize transformations
  if(sucess == false){
    this->reset();
  }

  //---------------------------
}
void CT_ICP::compute_updateLocation(Subset* subset){
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
  frame->is_slamed = true;

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
void CT_ICP::compute_statistics(float duration, Frame* frame, Frame* frame_m1, Subset* subset){
  voxelMap* map = mapManager->get_localmap();
  //---------------------------

  //Fill stats
  frame->time_slam = duration;
  frame->map_size_abs = map->size();
  frame->map_size_rlt = map->size() - map_size_old;
  this->map_size_old = map->size();

  //Transformation parameters
  vec3 trans_abs = vec3(frame->trans_b(0), frame->trans_b(1), frame->trans_b(2));
  vec3 trans_rlt;
  if(frame->ID == 0){
    trans_rlt = vec3(0, 0, 0);
  }else{
    trans_rlt.x = frame->trans_b(0) - frame_m1->trans_b(0);
    trans_rlt.y = frame->trans_b(1) - frame_m1->trans_b(1);
    trans_rlt.z = frame->trans_b(2) - frame_m1->trans_b(2);
  }

  Transforms transformManager;
  vec3 rotat_abs = transformManager.compute_anglesFromTransformationMatrix(frame->rotat_b);
  vec3 f0_rotat = transformManager.compute_anglesFromTransformationMatrix(frame->rotat_b);
  vec3 f1_rotat = transformManager.compute_anglesFromTransformationMatrix(frame_m1->rotat_b);

  vec3 rotat_rlt;
  if(frame->ID == 0){
    rotat_rlt = vec3(0, 0, 0);
  }
  else{
    rotat_rlt.x = f0_rotat.x - f0_rotat.x;
    rotat_rlt.y = f0_rotat.y - f1_rotat.y;
    rotat_rlt.z = f0_rotat.z - f1_rotat.z;
  }

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

//Support functions
void CT_ICP::reset(){
  //---------------------------

  mapManager->reset();
  this->map_frame_ID = 0;
  this->map_frame_begin_ID = 0;

  //---------------------------
}
void CT_ICP::set_nb_thread(int value){
  //---------------------------

  this->nb_thread = value;
  normalManager->set_nb_thread(nb_thread);
  gnManager->set_nb_thread(nb_thread);

  //---------------------------
}
