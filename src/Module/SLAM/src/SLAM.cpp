#include "SLAM.h"

#include "SLAM_sampling.h"
#include "SLAM_init.h"
#include "SLAM_assessment.h"
#include "SLAM_map.h"
#include "SLAM_parameter.h"
#include "SLAM_transform.h"

#include "../Glyph/SLAM_glyph.h"
#include "../optim/SLAM_normal.h"
#include "../optim/SLAM_optim.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"
#include "../../../Engine/Core/Configuration.h"
#include "../../../Specific/Function/fct_transtypage.h"
#include "../../../Specific/Function/fct_math.h"
#include "../../../Specific/Function/fct_chrono.h"


//Constructor / Destructor
SLAM::SLAM(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  Node_scene* node_scene = node_engine->get_node_scene();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_scene->get_sceneManager();

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
bool SLAM::compute_slam(Collection* collection, int subset_ID){
  Cloud* cloud = (Cloud*)collection->get_obj_byID(subset_ID);
  auto t1 = start_chrono();
  if(check_condition(collection, subset_ID) == false) return false;
  //---------------------------

  slam_init->compute_initialization(collection, subset_ID);
  slam_transf->compute_preprocessing(collection, subset_ID);
  slam_optim->compute_optimization(collection, subset_ID);

  //---------------------------
  time_slam = stop_chrono(t1);
  bool success = slam_assess->compute_assessment(collection, subset_ID, time_slam);
  this->compute_finalization(collection, subset_ID, success, time_slam);
  return success;
}
void SLAM::reset_slam(){
  //---------------------------

  slam_glyph->reset_glyph();
  slam_map->reset_map();

  //---------------------------
}

//Sub-functions
void SLAM::compute_finalization(Collection* collection, int subset_ID, bool success, float duration){
  Cloud* cloud = (Cloud*)collection->get_obj_byID(subset_ID);
  Frame* frame = collection->get_frame_byID(subset_ID);
  //---------------------------

  //Apply transformation
  if(success){
    slam_transf->transform_subset(cloud);
    slam_map->update_map(collection, subset_ID);
    slam_glyph->update_glyph(collection, cloud);
  //Else reset slam map
  }else{
    frame->reset();
    slam_map->reset_map();
    this->reset_visibility(collection, subset_ID);
  }

  //Compute SLAM statistiques
  slam_assess->compute_statistics(collection, subset_ID, duration);
  this->print_result();

  //---------------------------
}
bool SLAM::check_condition(Collection* collection, int subset_ID){
  Cloud* cloud = (Cloud*)collection->get_obj_byID(subset_ID);
  Frame* frame = collection->get_frame_byID(subset_ID);
  slamap* local_map = slam_map->get_local_map();
  //---------------------------

  //Cloud existence & consistency
  if(collection == nullptr){
    console.AddLog("error" ,"[SLAM] No collection");
    return false;
  }
  if(collection->ID_col_perma != local_map->linked_col_ID && local_map->linked_col_ID != -1){
    this->reset_slam();
  }

  //Subset number & timestamp
  if(subset_ID >= 2 && collection->list_obj.size() < 2){
    console.AddLog("error" ,"[SLAM] No enough subsets");
    return false;
  }
  if(cloud->has_timestamp == false){
    console.AddLog("error" ,"[SLAM] No timestamp");
    return false;
  }

  //Frame already slam computed
  if(frame->is_slam_made == true){
    slam_glyph->update_glyph(collection, cloud);
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
  if(lidar_model != collection->lidar_model){
    slam_param->make_config(collection->lidar_model);
    this->lidar_model = collection->lidar_model;
  }

  //---------------------------
  return true;
}
void SLAM::reset_visibility(Collection* collection, int subset_ID){
  //---------------------------

  //Set visibility just for last cloud
  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)collection->get_obj(i);

    if(cloud->ID == subset_ID){
      cloud->is_visible = true;
    }else{
      cloud->is_visible = false;
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
