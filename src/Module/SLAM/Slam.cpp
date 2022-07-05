#include "Slam.h"

//ID = subset absolute ID
//map_frame_ID = frame ID / the relative slam ID

#include "CT_ICP/SLAM_init.h"
#include "CT_ICP/SLAM_optim.h"
#include "CT_ICP/SLAM_assessment.h"
#include "CT_ICP/SLAM_map.h"
#include "CT_ICP/SLAM_parameter.h"

#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_chrono.h"
#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Object.h"
#include "../../Engine/Scene/Object/SLAM/Localmap.h"
#include "../../Engine/Scene/Configuration.h"


//Constructor / Destructor
Slam::Slam(Engine_node* node){
  this->node_engine = node;
  //---------------------------

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();

  this->mapManager = new SLAM_map(this);
  this->normalManager = new SLAM_normal(this);
  this->optimManager = new SLAM_optim(this);
  this->assessManager = new SLAM_assessment(this);
  this->paramManager = new SLAM_parameter(this);
  this->initManager = new SLAM_init(this);

  //---------------------------
  this->update_configuration();
}
Slam::~Slam(){}

//Main functions
void Slam::update_configuration(){
  //---------------------------

  this->offline_ID_max = 0;
  this->nb_thread = 8;

  string lidar_model = configManager->parse_json_s("interface", "lidar_model");
  paramManager->make_config(lidar_model);

  //---------------------------
}
void Slam::compute_slam_offline(Cloud* cloud){
  mapManager->reset_map_hard();
  if(cloud == nullptr) return;
  //---------------------------

  for(int i=0; i<offline_ID_max; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    Frame* frame_m0 = sceneManager->get_frame(cloud, i);
    Frame* frame_m1 = sceneManager->get_frame(cloud, i-1);
    auto t1 = high_resolution_clock::now();
    //--------------

    frame_m0->ID = i;

    initManager->compute_initialization(cloud, i);
    mapManager->compute_grid_sampling(subset);
    optimManager->compute_optimization(cloud, i);
    this->compute_assessment(cloud, i);

    //--------------
    auto t2 = high_resolution_clock::now();
    float duration = duration_cast<milliseconds>(t2 - t1).count();
    assessManager->compute_statistics(cloud, i, duration);
  }

  //---------------------------
}
void Slam::compute_slam_online(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  auto t1 = start_chrono();
  //---------------------------

  //Check SLAM conditions
  if(check_conditions(cloud, subset_ID) == false) return;

  //Main functions
  initManager->compute_initialization(cloud, subset_ID);
  mapManager->compute_grid_sampling(subset);
  optimManager->compute_optimization(cloud, subset_ID);
  this->compute_assessment(cloud, subset_ID);

  //---------------------------
  float duration = stop_chrono(t1);
  assessManager->compute_statistics(cloud, subset_ID, duration);
}

//SLAM sub-functions
bool Slam::compute_assessment(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  //---------------------------

  //Compute assessment
  bool success = assessManager->compute_assessment(cloud, subset_ID);

  //If unsuccess, reinitialize transformations
  if(success){
    mapManager->update_map(cloud, subset_ID);
    this->update_subset_location(subset);
    this->update_subset_glyph(subset);
  }else{
    /*initManager->init_frame_chain(cloud, subset_ID);
    mapManager->update_map(frame);
    this->update_subset_location(subset);
    this->update_subset_glyph(subset);*/

    frame->reset();
    mapManager->reset_map_smooth();
    this->reset_visibility(cloud, subset_ID);
  }

  //---------------------------
  return success;
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
<<<<<<< HEAD
    subset->keypoint.normal = Nxy;
    subset->keypoint.timestamp = ts;
=======
    subset->keypoint.timestamp = ts;
    subset->keypoint.normal = Nxy;
>>>>>>> tmp
  }

  //Update local map
  Localmap* mapObject = objectManager->get_object_localmap();
  mapObject->update_localmap(mapManager->get_slam_map());
  objectManager->update_object(mapObject->get_glyph());

  //---------------------------
  objectManager->update_glyph_subset(subset);
}
bool Slam::check_conditions(Cloud* cloud, int subset_ID){
  Subset* subset = sceneManager->get_subset_byID(cloud, subset_ID);
  Frame* frame = sceneManager->get_frame_byID(cloud, subset_ID);
  slamap* slam_map = mapManager->get_slam_map();
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
  if(slam_map->current_frame_ID == 0){
    slam_map->linked_subset_ID = subset_ID;
  }
  if(subset_ID < slam_map->linked_subset_ID) return false;
  //---> Check if the current selected cloud is the same than before
  if(cloud->ID != slam_map->linked_cloud_ID && slam_map->linked_cloud_ID != -1){
    this->reset_slam_hard();
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
void Slam::reset_slam_hard(){
  //---------------------------

  objectManager->reset_scene_object();
  mapManager->reset_map_hard();

  //---------------------------
}
