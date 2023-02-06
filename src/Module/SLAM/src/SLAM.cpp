#include "SLAM.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../Specific/fct_transtypage.h"
#include "../../../Specific/fct_math.h"
#include "../../../Specific/fct_chrono.h"

#include "SLAM_sampling.h"
#include "SLAM_init.h"
#include "SLAM_assessment.h"
#include "SLAM_map.h"
#include "SLAM_parameter.h"
#include "SLAM_transform.h"
#include "SLAM_glyph.h"

#include "../optim/SLAM_normal.h"
#include "../optim/SLAM_optim.h"


//Constructor / Destructor
SLAM::SLAM(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();

  this->slam_sampling = new SLAM_sampling();
  this->slam_map = new SLAM_map(this);
  this->slam_normal = new SLAM_normal(this);
  this->slam_optim = new SLAM_optim(this);
  this->slam_assess = new SLAM_assessment(this);
  this->slam_transf = new SLAM_transform(this);
  this->slam_param = new SLAM_parameter(this);
  this->slam_init = new SLAM_init(this);
  this->slam_glyph = new SLAM_glyph(this);

  //---------------------------
  this->update_configuration();
}
SLAM::~SLAM(){}

//Main functions
void SLAM::update_configuration(){
  //---------------------------

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
  slam_transf->compute_preprocessing(cloud, subset_ID);
  slam_optim->compute_optimization(cloud, subset_ID);

  //---------------------------
  time_slam = stop_chrono(t1);
  bool success = slam_assess->compute_assessment(cloud, subset_ID, time_slam);
  this->compute_finalization(cloud, subset_ID, success, time_slam);
  return success;
}
void SLAM::reset_slam(){
  //---------------------------

  slam_glyph->reset_glyph();
  slam_map->reset_map();

  //---------------------------
}

//Sub-functions
void SLAM::compute_finalization(Cloud* cloud, int subset_ID, bool success, float duration){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  //---------------------------

  //Apply transformation
  if(success){
    slam_transf->transform_subset(subset);
    slam_map->update_map(cloud, subset_ID);
    slam_glyph->update_glyph(cloud, subset);
  //Else reset slam map
  }else{
    frame->reset();
    slam_map->reset_map();
    this->reset_visibility(cloud, subset_ID);
  }

  //Compute SLAM statistiques
  slam_assess->compute_statistics(cloud, subset_ID, duration);
  this->print_result();

  //---------------------------
}
bool SLAM::check_condition(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
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
  if(subset_ID >= 2 && cloud->subset.size() < 2){
    console.AddLog("error" ,"[SLAM] No enough subsets");
    return false;
  }
  if(subset->has_timestamp == false){
    console.AddLog("error" ,"[SLAM] No timestamp");
    return false;
  }

  //Frame already slam computed
  if(frame->is_slam_made == true){
    slam_glyph->update_glyph(cloud, subset);
    return false;
  }

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

  //Check lidar model
  if(lidar_model != cloud->lidar_model){
    slam_param->make_config(cloud->lidar_model);
    this->lidar_model = cloud->lidar_model;
  }

  //---------------------------
  return true;
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
void SLAM::print_result(){
  //---------------------------

  string path = "../src/Module/SLAM/result.dat";
  string result = to_string(time_slam);

  ofstream file;
  file.open(path, std::ofstream::out | std::ofstream::trunc);
  file << result;
  file.close();

  //---------------------------
}
