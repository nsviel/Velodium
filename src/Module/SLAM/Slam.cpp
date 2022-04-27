#include "Slam.h"

//ID = subset absolute ID
//map_frame_ID = frame ID / the relative slam ID

#include "CT_ICP/SLAM_optim_gn.h"
#include "CT_ICP/SLAM_assessment.h"
#include "CT_ICP/SLAM_localMap.h"
#include "CT_ICP/SLAM_parameter.h"

#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"
#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Object.h"
#include "../../Engine/Scene/Object/Trajectory.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Operation/Transformation/Transforms.h"

#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;


//Constructor / Destructor
Slam::Slam(Engine_node* node){
  this->node_engine = node;
  //---------------------------

  Configuration* configManager = node_engine->get_configManager();

  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();

  this->normalManager = new SLAM_normal();
  //this->ceresManager = new SLAM_optim_ceres(normalManager);
  this->gnManager = new SLAM_optim_gn(normalManager);
  this->assessManager = new SLAM_assessment(this);
  this->mapManager = new SLAM_localMap();
  this->paramManager = new SLAM_parameter(this);

  this->solver_ceres = false;
  this->solver_GN = true;
  this->verbose = false;
  this->ID_all = true;

  this->ID_max = 0;
  this->ID_cloud = -1; //To ensure that the current processing cloud is the same than before
  this->map_frame_ID = 0;
  this->map_frame_begin_ID = 0;
  this->map_size_old = 0;
  this->nb_thread = 8;

  paramManager->make_config(configManager->parse_json_s("interface", "lidar_model"));

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
    auto t1 = high_resolution_clock::now();
    //--------------

    frame_m0->ID = i;

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
    auto t2 = high_resolution_clock::now();
    float duration = duration_cast<milliseconds>(t2 - t1).count();
    this->compute_statistics(duration, frame_m0, frame_m1, subset);
  }

  mapManager->end_slamVoxelization(cloud, ID_max);
  //---------------------------
}
void Slam::compute_slam_online(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, subset_ID-1);
  Frame* frame_m2 = sceneManager->get_frame_byID(cloud, subset_ID-2);

  //SLAM algorithm
  if(check_conditions(cloud, subset_ID) == false) return;
  auto t1 = high_resolution_clock::now();
  //---------------------------

  this->init_frameID(cloud, subset_ID);
  this->init_frameTimestamp(subset);
  this->init_frameChain(frame, frame_m1, frame_m2);
  this->init_distortion(frame);

  mapManager->compute_gridSampling(subset);

  this->compute_optimization(frame, frame_m1);
  this->compute_assessment(cloud, subset_ID);

  mapManager->add_pointsToLocalMap(frame);
  mapManager->end_clearTooFarVoxels(frame->trans_e);

  this->compute_updateLocation(subset);

  //---------------------------
  auto t2 = high_resolution_clock::now();
  float duration = duration_cast<milliseconds>(t2 - t1).count();
  this->compute_statistics(duration, frame, frame_m1, subset);
  objectManager->update_glyph_subset(subset);
}

//SLAM sub-functions
bool Slam::check_conditions(Cloud* cloud, int subset_ID){
  //Check for computing conditions
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  //---------------------------

  //Data error
  if(subset->xyz.size() == 0){
    console.AddLog("error" ,"SLAM - no data points");
    return false;
  }
  if(subset_ID >= 2 && cloud->subset.size() < 2){
    console.AddLog("error" ,"SLAM - no enough subsets");
    return false;
  }
  if(subset->has_timestamp == false){
    console.AddLog("error" ,"SLAM - no subset timestamp");
    return false;
  }

  //No ponctual SLAM condition
  if(frame->is_slamed == true) return false;
  if(map_frame_ID == 0){
    this->map_frame_begin_ID = subset_ID;
  }
  if(subset_ID < map_frame_begin_ID) return false;
  if(cloud->ID != ID_cloud && ID_cloud != -1){
    this->reset_slam();
  }

  //---------------------------
  return true;
}
void Slam::init_frameID(Cloud* cloud, int subset_ID){
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  //---------------------------

  frame->ID = map_frame_ID;
  this->ID_cloud = cloud->ID;
  this->map_frame_ID++;

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
    frame->rotat_b = Eigen::Matrix3f::Identity();
    frame->rotat_e = Eigen::Matrix3f::Identity();
    frame->trans_b = Eigen::Vector3f::Zero();
    frame->trans_e = Eigen::Vector3f::Zero();
  }
  //Second frame
  else if(frame->ID == 2){
    // Different for the second frame due to the bootstrapped elasticity
    Eigen::Matrix3f rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3f trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    frame->rotat_b = frame_m1->rotat_e;
    frame->trans_b = frame_m1->trans_e;
    frame->rotat_e = rotat_next_e;
    frame->trans_e = trans_next_e;
  }
  //Other frame
  else if(frame->ID > 2){
    // When continuous: use the previous begin_pose as reference
    Eigen::Matrix3f rotat_next_b = frame_m1->rotat_b * frame_m2->rotat_b.inverse() * frame_m1->rotat_b;
    Eigen::Vector3f trans_next_b = frame_m1->trans_b + frame_m1->rotat_b * frame_m2->rotat_b.inverse() * (frame_m1->trans_b - frame_m2->trans_b);
    Eigen::Matrix3f rotat_next_e = frame_m1->rotat_e * frame_m2->rotat_e.inverse() * frame_m1->rotat_e;
    Eigen::Vector3f trans_next_e = frame_m1->trans_e + frame_m1->rotat_e * frame_m2->rotat_e.inverse() * (frame_m1->trans_e - frame_m2->trans_e);

    frame->rotat_b = rotat_next_b;
    frame->trans_b = trans_next_b;
    frame->rotat_e = rotat_next_e;
    frame->trans_e = trans_next_e;
  }

  //---------------------------
}
void Slam::init_distortion(Frame* frame){
  //---------------------------

  if(frame->ID > 1){
    Eigen::Quaternionf quat_b = Eigen::Quaternionf(frame->rotat_b);
    Eigen::Quaternionf quat_e = Eigen::Quaternionf(frame->rotat_e);
    Eigen::Vector3f trans_b = frame->trans_b;
    Eigen::Vector3f trans_e = frame->trans_e;

    // Distorts the frame (put all raw_points in the coordinate frame of the pose at the end of the acquisition)
    Eigen::Quaternionf quat_e_inv = quat_e.inverse(); // Rotation of the inverse pose
    Eigen::Vector3f trans_e_inv = -1.0 * (quat_e_inv * trans_e); // Translation of the inverse pose

    for (int i=0; i < frame->xyz.size(); i++) {

      float ts_n = frame->ts_n[i];
      Eigen::Vector3f& point = frame->xyz_raw[i];

      Eigen::Quaternionf quat_n = quat_b.slerp(ts_n, quat_e).normalized();
      Eigen::Vector3f t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

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
      //ceresManager->optim_test(frame, frame_m1, map);
    }
  }

  //---------------------------
}
void Slam::compute_assessment(Cloud* cloud, int subset_ID){
  //---------------------------

  //Compute assessment
  bool success = true;
  success = assessManager->compute_assessment(cloud, subset_ID);

  //If unsuccess, reinitialize transformations
  if(success == false){
    Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
    frame->reset();
    this->reset_slam();
  }

  //---------------------------
}
void Slam::compute_updateLocation(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  Eigen::Quaternionf quat_b = Eigen::Quaternionf(frame->rotat_b);
  Eigen::Quaternionf quat_e = Eigen::Quaternionf(frame->rotat_e);
  Eigen::Vector3f trans_b = frame->trans_b;
  Eigen::Vector3f trans_e = frame->trans_e;

  //Update frame root
  Eigen::Matrix3f R = quat_b.toRotationMatrix();
  Eigen::Vector3f t = trans_b;
  Eigen::Vector3f root = R * glm_to_eigen_vec3_d(subset->root) + t;
  subset->root = eigen_to_glm_vec3_d(root);
  frame->is_slamed = true;

  //Update subset position
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<subset->xyz.size(); i++){
    //Compute paramaters
    float ts_n = subset->ts_n[i];
    Eigen::Matrix3f R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3f t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    //Apply transformation
    Eigen::Vector3f point {subset->xyz[i].x, subset->xyz[i].y, subset->xyz[i].z};
    point = R * point + t;
    subset->xyz[i] = vec3(point(0), point(1), point(2));
  }

  //---------------------------
  sceneManager->update_subset_location(subset);
}
void Slam::compute_statistics(float duration, Frame* frame_m0, Frame* frame_m1, Subset* subset){
  voxelMap* map = mapManager->get_localmap();
  //---------------------------

  //Fill stats
  frame_m0->time_slam = duration;
  frame_m0->map_size_abs = map->size();
  frame_m0->map_size_rlt = map->size() - map_size_old;
  this->map_size_old = map->size();

  //Transformation parameters
  vec3 trans_abs = vec3(frame_m0->trans_b(0), frame_m0->trans_b(1), frame_m0->trans_b(2));
  vec3 trans_rlt;
  if(frame_m0->ID == 0){
    trans_rlt = vec3(0, 0, 0);
  }else if(frame_m1 != nullptr){
    trans_rlt.x = frame_m0->trans_b(0) - frame_m1->trans_b(0);
    trans_rlt.y = frame_m0->trans_b(1) - frame_m1->trans_b(1);
    trans_rlt.z = frame_m0->trans_b(2) - frame_m1->trans_b(2);
  }

  Transforms transformManager;
  vec3 rotat_abs = transformManager.compute_anglesFromTransformationMatrix(frame_m0->rotat_b);

  vec3 rotat_rlt;
  if(frame_m0->ID == 0){
    rotat_rlt = vec3(0, 0, 0);
  }
  else if(frame_m1 != nullptr){
    vec3 f0_rotat = transformManager.compute_anglesFromTransformationMatrix(frame_m0->rotat_b);
    vec3 f1_rotat = transformManager.compute_anglesFromTransformationMatrix(frame_m1->rotat_b);
    rotat_rlt = f0_rotat - f1_rotat;
  }

  frame_m0->trans_abs = trans_abs;
  frame_m0->rotat_abs = rotat_abs;
  frame_m0->trans_rlt = trans_rlt;
  frame_m0->rotat_rlt = rotat_rlt;

  //Terminal result
  if(verbose){
    cout<<"[success] SLAM - "<<subset->name.c_str();
    cout<<" "<<to_string(frame_m0->ID)<<"/"<< ID_max;
    cout<< " [" <<frame_m0->time_slam<< " ms]"<<endl;
  }

  //---------------------------
}

//Support functions
void Slam::reset_slam(){
  //---------------------------

  objectManager->reset_scene_object();
  mapManager->reset();
  this->map_frame_ID = 0;

  //---------------------------
}
