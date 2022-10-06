#include "SLAM.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Object.h"
#include "../../Engine/Scene/Object/SLAM/Localmap.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_chrono.h"

#include "src/SLAM_init.h"
#include "src/SLAM_assessment.h"
#include "src/SLAM_map.h"
#include "src/SLAM_parameter.h"
#include "src/SLAM_normal.h"
#include "optim/SLAM_optim.h"


//Constructor / Destructor
SLAM::SLAM(Engine_node* node){
  this->node_engine = node;
  //---------------------------

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();

  this->slam_map = new SLAM_map(this);
  this->slam_normal = new SLAM_normal(this);
  this->slam_optim = new SLAM_optim(this);
  this->slam_assess = new SLAM_assessment(this);
  this->slam_param = new SLAM_parameter(this);
  this->slam_init = new SLAM_init(this);

  //---------------------------
  this->update_configuration();
}
SLAM::~SLAM(){}

//Main functions
void SLAM::update_configuration(){
  //---------------------------

  this->offline_ID_max = 0;
  this->nb_thread = 8;
  this->lidar_model = configManager->parse_json_s("interface", "lidar_model");
  slam_param->make_config(lidar_model);

  //---------------------------
}
bool SLAM::compute_slam(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  auto t1 = start_chrono();
  if(check_condition(cloud, subset_ID) == false) return false;
  //---------------------------

  slam_init->compute_initialization(cloud, subset_ID);
  slam_map->compute_grid_sampling(subset);
  slam_optim->compute_optimization(cloud, subset_ID);
  bool success = slam_assess->compute_assessment(cloud, subset_ID);

  //---------------------------
  float duration = stop_chrono(t1);
  this->compute_finalization(cloud, subset_ID, success, duration);
  return success;
}
void SLAM::reset_slam(){
  //---------------------------

  objectManager->reset_scene_object();
  slam_map->reset_map_hard();

  //---------------------------
}

//Sub-functions
void SLAM::compute_finalization(Cloud* cloud, int subset_ID, bool success, float duration){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  //---------------------------

  //Apply transformation
  if(success){
    slam_map->update_map(cloud, subset_ID);
    this->update_subset_location(subset);
    this->update_subset_glyph(subset);
  //Else reset slam map
  }else{
    /*slam_init->init_frame_chain(cloud, subset_ID);
    slam_map->update_map(frame);
    this->update_subset_location(subset);
    this->update_subset_glyph(subset);*/

    frame->reset();
    slam_map->reset_map_smooth();
    this->reset_visibility(cloud, subset_ID);
  }

  //Compute SLAM statistiques
  slam_assess->compute_statistics(cloud, subset_ID, duration);

  //---------------------------
}
bool SLAM::check_condition(Cloud* cloud, int subset_ID){
  slamap* local_map = slam_map->get_local_map();
  //---------------------------

  //Cloud existence & consistency
  if(cloud == nullptr){
    console.AddLog("error" ,"[SLAM] No cloud");
    return false;
  }
  if(cloud->ID != local_map->linked_cloud_ID && local_map->linked_cloud_ID != -1){
    this->reset_slam();
  }

  //Subset number & timestamp
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  if(subset_ID >= 2 && cloud->subset.size() < 2){
    console.AddLog("error" ,"[SLAM] No enough subsets");
    return false;
  }
  if(subset->has_timestamp == false){
    console.AddLog("error" ,"[SLAM] No subset timestamp");
    return false;
  }

  //Frame already slam computed
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  if(frame->is_slamed == true) return false;

  //Local map
  if(subset_ID < local_map->linked_subset_ID){
    return false;
  }
  if(local_map->current_frame_ID == 0){
    local_map->linked_subset_ID = subset_ID;
  }
  if(subset_ID < local_map->linked_subset_ID){
    return false;
  }

  //---------------------------
  return true;
}
void SLAM::update_subset_location(Subset* subset){
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
void SLAM::update_subset_glyph(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Update keypoint
  if(frame->xyz.size() == frame->nn.size()){
    vector<vec3> xyz;
    vector<vec3> Nxy;
    vector<float> ts;

    for(int i=0; i<frame->xyz.size(); i++){
      if(isnan(frame->nn[i](0)) == false){
        xyz.push_back(vec3(frame->nn[i](0), frame->nn[i](1), frame->nn[i](2)));
        Nxy.push_back(vec3(frame->N_nn[i](0), frame->N_nn[i](1), frame->N_nn[i](2)));
        ts.push_back(frame->ts_n[i]);
      }
    }

    subset->keypoint.location = xyz;
    subset->keypoint.timestamp = ts;
    subset->keypoint.normal = Nxy;
  }

  //Update local map
  Localmap* mapObject = objectManager->get_object_localmap();
  mapObject->update_localmap(slam_map->get_local_map());
  objectManager->update_object(mapObject->get_glyph());

  //---------------------------
  objectManager->update_glyph_subset(subset);
}
void SLAM::reset_visibility(Cloud* cloud, int subset_ID){
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
