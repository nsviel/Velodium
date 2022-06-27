#include "Slam.h"

//ID = subset absolute ID
//map_frame_ID = frame ID / the relative slam ID

#include "CT_ICP/SLAM_init.h"
#include "CT_ICP/SLAM_optim_gn.h"
#include "CT_ICP/SLAM_assessment.h"
#include "CT_ICP/SLAM_localMap.h"
#include "CT_ICP/SLAM_parameter.h"

#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"
#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Object.h"
#include "../../Engine/Scene/Object/SLAM/Localmap.h"
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

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();

  this->normalManager = new SLAM_normal();
  this->gnManager = new SLAM_optim_gn(this);
  this->assessManager = new SLAM_assessment(this);
  this->mapManager = new SLAM_localMap();
  this->paramManager = new SLAM_parameter(this);
  this->initManager = new SLAM_init(this);

  //---------------------------
  this->update_configuration();
}
Slam::~Slam(){}

//Main functions
void Slam::update_configuration(){
  //---------------------------

  this->solver_ceres = false;
  this->solver_GN = true;
  this->verbose = false;
  this->with_distorsion = true;

  this->offline_ID_max = 0;
  this->map_frame_begin_ID = 0;
  this->map_size_old = 0;
  this->nb_thread = 8;

  string lidar_model = configManager->parse_json_s("interface", "lidar_model");
  paramManager->make_config(lidar_model);

  //---------------------------
}
void Slam::compute_slam_offline(Cloud* cloud){
  mapManager->reset_map();
  if(cloud == nullptr) return;
  //---------------------------

  for(int i=0; i<offline_ID_max; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    Frame* frame_m0 = sceneManager->get_frame(cloud, i);
    Frame* frame_m1 = sceneManager->get_frame(cloud, i-1);
    Frame* frame_m2 = sceneManager->get_frame(cloud, i-2);
    auto t1 = high_resolution_clock::now();
    //--------------

    frame_m0->ID = i;

    initManager->compute_initialization(cloud, i);
    mapManager->compute_grid_sampling(subset);

    this->compute_optimization(frame_m0, frame_m1);
    this->compute_assessment(cloud, i);

    mapManager->add_pointsToSlamMap(subset);
    mapManager->add_pointsToLocalMap(frame_m0);
    mapManager->end_clearTooFarVoxels(frame_m0->trans_e);

    this->update_subset_location(subset);

    //--------------
    auto t2 = high_resolution_clock::now();
    float duration = duration_cast<milliseconds>(t2 - t1).count();
    this->compute_statistics(duration, frame_m0, frame_m1, subset);
  }

  mapManager->end_slamVoxelization(cloud, offline_ID_max);
  //---------------------------
}
void Slam::compute_slam_online(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, subset_ID-1);
  auto t1 = high_resolution_clock::now();
  //---------------------------

  //Check SLAM conditions
  if(check_conditions(cloud, subset_ID) == false) return;

  //Initialization
  initManager->compute_initialization(cloud, subset_ID);
  mapManager->compute_grid_sampling(subset);

  //Main computing part
  this->compute_distortion(frame);
  this->compute_optimization(frame, frame_m1);
  bool success = this->compute_assessment(cloud, subset_ID);

  //End functions
  if(success){
    mapManager->add_pointsToLocalMap(frame);
    mapManager->end_clearTooFarVoxels(frame->trans_e);
    this->update_subset_location(subset);
    this->update_subset_glyph(subset);
  }

  //---------------------------
  auto t2 = high_resolution_clock::now();
  float duration = duration_cast<milliseconds>(t2 - t1).count();
  this->compute_statistics(duration, frame, frame_m1, subset);
}

//SLAM sub-functions
void Slam::compute_distortion(Frame* frame){
  //---------------------------

  if(with_distorsion && frame->ID >= 2){
    Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
    Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
    Eigen::Vector3d trans_b = frame->trans_b;
    Eigen::Vector3d trans_e = frame->trans_e;

    //Distorts the frame
    Eigen::Quaterniond quat_e_inv = quat_e.inverse();
    Eigen::Vector3d trans_e_inv = -1.0 * (quat_e_inv * trans_e);

    for(int i=0; i<frame->xyz.size(); i++){
      float ts_n = frame->ts_n[i];

      Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
      Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

      // Distort Raw Keypoints
      frame->xyz[i] = (R * frame->xyz_raw[i] + t) + (t + trans_e_inv);
    }
  }

  //---------------------------
}
void Slam::compute_optimization(Frame* frame, Frame* frame_m1){
  voxelMap* map = mapManager->get_map_local();
  //---------------------------

  if(frame->ID > 0){
    if(solver_GN){
      gnManager->optim_GN(frame, frame_m1, map);
    }else if(solver_ceres){
      //ceresManager->optim_test(frame, frame_m1, map);
    }
  }

  //---------------------------
}
bool Slam::compute_assessment(Cloud* cloud, int subset_ID){
  //---------------------------

  //Compute assessment
  bool success = true;
  success = assessManager->compute_assessment(cloud, subset_ID);

  //If unsuccess, reinitialize transformations
  if(success == false){
    Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
    //frame->reset();
    mapManager->reset_map();
    //this->reset_slam();
    //this->reset_visibility(cloud, subset_ID);
  }

  //---------------------------
  return success;
}
void Slam::compute_statistics(float duration, Frame* frame_m0, Frame* frame_m1, Subset* subset){
  voxelMap* map = mapManager->get_map_local();
  Transforms transformManager;
  //---------------------------

  //Fill stats
  frame_m0->time_slam = duration;
  frame_m0->map_size_abs = map->size();
  frame_m0->map_size_rlt = map->size() - map_size_old;
  this->map_size_old = map->size();

  //Relative parameters
  vec3 trans_b_rlt, trans_e_rlt;
  if(frame_m1 != nullptr && frame_m0->ID != 0){
    for(int i=0; i<3; i++){
      trans_b_rlt[i] = frame_m0->trans_b(i) - frame_m1->trans_e(i);
      trans_e_rlt[i] = frame_m0->trans_e(i) - frame_m0->trans_b(i);
    }
  }

  vec3 rotat_b_rlt, rotat_e_rlt;
  if(frame_m1 != nullptr && frame_m0->ID != 0){
    vec3 a1_b = transformManager.compute_anglesFromTransformationMatrix(frame_m0->rotat_b);
    vec3 a2_b = transformManager.compute_anglesFromTransformationMatrix(frame_m1->rotat_e);
    rotat_b_rlt = a1_b - a2_b;

    vec3 a1_e = transformManager.compute_anglesFromTransformationMatrix(frame_m0->rotat_e);
    vec3 a2_e = transformManager.compute_anglesFromTransformationMatrix(frame_m0->rotat_b);
    rotat_e_rlt = a1_e - a2_e;
  }

  frame_m0->trans_b_rlt = trans_b_rlt;
  frame_m0->rotat_b_rlt = rotat_b_rlt;
  frame_m0->trans_e_rlt = trans_e_rlt;
  frame_m0->rotat_e_rlt = rotat_e_rlt;
  frame_m0->angle_b = transformManager.compute_anglesFromTransformationMatrix(frame_m0->rotat_b);
  frame_m0->angle_e = transformManager.compute_anglesFromTransformationMatrix(frame_m0->rotat_e);

  //Terminal result
  if(verbose){
    cout<<"[success] SLAM - "<<subset->name.c_str();
    cout<<" "<<to_string(frame_m0->ID)<<"/"<< offline_ID_max;
    cout<< " [" <<frame_m0->time_slam<< " ms]"<<endl;
  }

  //---------------------------
}

void Slam::update_subset_location(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  //Update frame root
  subset->rotat = eigen_to_glm_mat4(quat_b.toRotationMatrix());
  subset->root = eigen_to_glm_vec3(trans_b);
  frame->is_slamed = true;

  //Update subset position
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<subset->xyz.size(); i++){
    //Compute paramaters
    float ts_n = subset->ts_n[i];
    Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    //Apply transformation
    Eigen::Vector3d point {subset->xyz[i].x, subset->xyz[i].y, subset->xyz[i].z};
    point = R * point + t;
    subset->xyz[i] = vec3(point(0), point(1), point(2));
  }

  //---------------------------
  sceneManager->update_subset_location(subset);
}
void Slam::update_subset_glyph(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Update keypoint
  subset->keypoint.location = eigen_to_glm_vectorVec3(frame->nn, nb_thread);
  subset->keypoint.timestamp = vec_double_to_float(frame->ts_n);
  if(frame->N_nn.size() == frame->xyz.size()){
    subset->keypoint.normal = eigen_to_glm_vectorVec3(frame->N_nn, nb_thread);
  }

  //Update local map
  Localmap* mapObject = objectManager->get_object_localmap();
  mapObject->update_localmap(mapManager->get_map_local());
  objectManager->update_object(mapObject->get_glyph());

  //---------------------------
  objectManager->update_glyph_subset(subset);
}

bool Slam::check_conditions(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  //---------------------------

  //Data error
  if(subset->xyz.size() == 0){
    console.AddLog("error" ,"SLAM - No data points");
    return false;
  }
  if(subset_ID >= 2 && cloud->subset.size() < 2){
    console.AddLog("error" ,"SLAM - No enough subsets");
    return false;
  }
  if(subset->has_timestamp == false){
    console.AddLog("error" ,"SLAM - No subset timestamp");
    return false;
  }

  //No ponctual SLAM condition
  if(frame->is_slamed == true) return false;
  int map_frame_ID = initManager->get_frame_ID();
  if(map_frame_ID == 0){
    this->map_frame_begin_ID = subset_ID;
  }
  if(subset_ID < map_frame_begin_ID) return false;
  //---> Check if the current selected cloud is the same than before
  int ID_cloud = initManager->get_ID_cloud();
  if(cloud->ID != ID_cloud && ID_cloud != -1){
    this->reset_slam();
  }

  //---------------------------
  return true;
}
void Slam::reset_visibility(Cloud* cloud, int subset_ID){
  //---------------------------

  //Set visibility just for last subset
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->ID == subset_ID){
      subset->visibility = true;
    }else{
      subset->visibility = false;
    }
  }

  //---------------------------
}
void Slam::reset_slam(){
  //---------------------------

  //Reset SLAM objects
  objectManager->reset_scene_object();
  mapManager->reset_map();
  initManager->set_frame_ID(0);

  //---------------------------
}
