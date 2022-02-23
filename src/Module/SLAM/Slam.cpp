#include "Slam.h"

#include "CT_ICP/SLAM_optim_ceres.h"
#include "CT_ICP/SLAM_optim_gn.h"
#include "CT_ICP/SLAM_assessment.h"
#include "CT_ICP/SLAM_localMap.h"
#include "CT_ICP/SLAM_configuration.h"

#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Glyphs.h"
#include "../../Operation/Transformation/Transforms.h"


//Constructor / Destructor
Slam::Slam(){
  //---------------------------

  this->sceneManager = new Scene();
  this->glyphManager = new Glyphs();

  this->normalManager = new SLAM_normal();
  this->ceresManager = new SLAM_optim_ceres(normalManager);
  this->gnManager = new SLAM_optim_gn(normalManager);
  this->assessManager = new SLAM_assessment(gnManager);
  this->mapManager = new SLAM_localMap();
  this->configManager = new SLAM_configuration(this);

  this->solver_ceres = false;
  this->solver_GN = true;
  this->verbose = false;
  this->ID_all = true;

  this->ID_max = 0;
  this->map_frame_ID = 0;
  this->map_frame_begin_ID = 0;
  this->map_size_old = 0;
  this->nb_thread = 8;

  configManager->make_config(0);

  //---------------------------
}
Slam::~Slam(){}

//Main functions
void Slam::compute_slam(Cloud* cloud){
  mapManager->reset();
  if(cloud == nullptr) return;
  if(ID_all) ID_max = sceneManager->get_subset(cloud, cloud->nb_subset-1)->ID;
  //---------------------------

  for(int i=0; i<ID_max; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    Frame* frame_m0 = sceneManager->get_frame(cloud, i);
    Frame* frame_m1 = sceneManager->get_frame(cloud, i-1);
    Frame* frame_m2 = sceneManager->get_frame(cloud, i-2);
    frame_m0->ID = i;
    tic();
    //--------------

    this->init_frameTimestamp(subset);
    this->init_frameChain(frame_m0, frame_m1, frame_m2);
    this->init_distortion(frame_m0);

    mapManager->compute_gridSampling(subset);

    this->compute_optimization(frame_m0, frame_m1);
    this->compute_assessment(cloud, i);

    mapManager->add_pointsToSlamMap(subset);
    mapManager->add_pointsToLocalMap(frame_m0);
    mapManager->end_clearTooFarVoxels(frame_m0->trans_e);

    this->compute_updateLocation(subset);

    //--------------
    float duration = toc();
    this->compute_statistics(duration, frame_m0, frame_m1, subset);
  }

  mapManager->end_slamVoxelization(cloud, ID_max);

  //---------------------------
}
void Slam::compute_slam_online(Cloud* cloud, int ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, ID);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, ID-1);
  Frame* frame_m2 = sceneManager->get_frame_byID(cloud, ID-2);
  //---------------------------

  //Check for computing conditions
  if(subset->xyz.size() == 0) return;
  if(ID >= 2 && cloud->subset.size() < 2) return;
  if(frame->is_slamed == true) return;
  if(ID < map_frame_begin_ID) return;


  //SLAM algorithm
  tic();
  //---------------------------

  //Bien checker pour chaque functions si map_frame_ID est bien respecté

  this->init_frameID(frame, ID);
  this->init_frameTimestamp(subset);
  this->init_frameChain(frame, frame_m1, frame_m2);
  this->init_distortion(frame);

  mapManager->compute_gridSampling(subset);

  this->compute_optimization(frame, frame_m1);
  this->compute_assessment(cloud, ID);

  mapManager->add_pointsToLocalMap(frame);
  mapManager->end_clearTooFarVoxels(frame->trans_e);

  this->compute_updateLocation(subset);

  //---------------------------
  float duration = toc();
  this->compute_statistics(duration, frame, frame_m1, subset);
  glyphManager->update(subset);
}

//SLAM sub-functions
void Slam::init_frameID(Frame* frame, int ID){
  //---------------------------

  if(map_frame_ID == 0){
    map_frame_begin_ID = ID;
  }

  frame->ID = map_frame_ID;

  map_frame_ID++;

  //---------------------------
}
void Slam::init_frameTimestamp(Subset* subset){
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
void Slam::init_frameChain(Frame* frame, Frame* frame_m1, Frame* frame_m2){
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
void Slam::init_distortion(Frame* frame){
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

void Slam::compute_optimization(Frame* frame, Frame* frame_m1){
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
void Slam::compute_assessment(Cloud* cloud, int ID){
  bool sucess = true;
  //---------------------------

  if(map_frame_ID > 5){
    sucess = assessManager->compute_assessment(cloud, ID);
  }

  //If unsucess, reinitialize transformations
  if(sucess == false){
    Frame* frame = sceneManager->get_frame_byID(cloud, ID);
    frame->reset();
    this->reset_slam();
  }

  //---------------------------
}
void Slam::compute_updateLocation(Subset* subset){
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
void Slam::compute_statistics(float duration, Frame* frame, Frame* frame_m1, Subset* subset){
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
  }else if(frame_m1 != nullptr){
    trans_rlt.x = frame->trans_b(0) - frame_m1->trans_b(0);
    trans_rlt.y = frame->trans_b(1) - frame_m1->trans_b(1);
    trans_rlt.z = frame->trans_b(2) - frame_m1->trans_b(2);
  }

  Transforms transformManager;
  vec3 rotat_abs = transformManager.compute_anglesFromTransformationMatrix(frame->rotat_b);

  vec3 rotat_rlt;
  if(frame->ID == 0){
    rotat_rlt = vec3(0, 0, 0);
  }
  else if(frame_m1 != nullptr){
    vec3 f0_rotat = transformManager.compute_anglesFromTransformationMatrix(frame->rotat_b);
    vec3 f1_rotat = transformManager.compute_anglesFromTransformationMatrix(frame_m1->rotat_b);

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
    cout<<" "<<to_string(frame->ID)<<"/"<< ID_max;
    cout<< " [" <<frame->time_slam<< " ms]"<<endl;
  }

  //---------------------------
}

//Support functions
void Slam::reset_slam(){
  //---------------------------

  mapManager->reset();
  this->map_frame_ID = 0;
  this->map_frame_begin_ID = 0;

  //---------------------------
}
