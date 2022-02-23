#include "Scenario.h"

#include "Interface/Interfacing.h"
#include "Interface/Prediction.h"
#include "Interface/GPS.h"

#include "../Module_node.h"
#include "../LiDAR/Capture.h"
#include "../Network/Network.h"
#include "../SLAM/Slam.h"
#include "../SLAM/CT_ICP/SLAM_configuration.h"
#include "../Player/Player_online.h"


//Constructor / Destructor
Scenario::Scenario(Module_node* node_module){
  //---------------------------

  Slam* slamManager = node_module->get_slamManager();

  this->ioManager = node_module->get_ioManager();
  this->captureManager = node_module->get_captureManager();
  this->netManager = node_module->get_netManager();
  this->onlineManager = node_module->get_onlineManager();
  this->slam_configManager = slamManager->get_slam_config();
  this->predManager = ioManager->get_predManager();
  this->gpsManager = ioManager->get_gpsManager();

  this->scenario_selected = 1;
  this->scenario_started = false;

  //---------------------------
}
Scenario::~Scenario(){}

//Run scenario functions
void Scenario::scenario_start(){
  //---------------------------

  slam_configManager->set_predefined_conf(1);
  this->make_configuration();
  this->make_watcher();

  //---------------------------
}
void Scenario::scenario_stop(){
  //---------------------------

  predManager->stop_watcher_prediction();
  gpsManager->stop_watcher_gps();

  //---------------------------
}

//Subfunctions
void Scenario::make_configuration(){
  //---------------------------

  //Parameters
  bool* with_camera = onlineManager->get_with_camera_follow();
  bool* with_slam = onlineManager->get_with_slam();
  bool* with_cylinder = onlineManager->get_with_cylinder_filter();
  bool* with_heatmap = onlineManager->get_with_heatmap();
  bool* with_save_frame = onlineManager->get_with_save_frame();
  bool* with_save_image = onlineManager->get_with_save_image();

  //Set according to scenario choice
  switch(scenario_selected){
    case 0:{
      *with_camera = false;
      *with_slam = false;
      *with_cylinder = false;
      *with_heatmap = false;
      *with_save_frame = false;
      *with_save_image = false;
      break;
    }

    case 1:{
      *with_camera = true;
      *with_slam = true;
      *with_cylinder = true;
      *with_heatmap = true;
      *with_save_frame = true;
      *with_save_image = true;
      break;
    }

    case 2:
    case 3:{
      *with_camera = false;
      *with_slam = true;
      *with_cylinder = true;
      *with_heatmap = false;
      *with_save_frame = true;
      *with_save_image = false;
      break;
    }
  }

  //---------------------------
}
void Scenario::make_watcher(){
  //---------------------------

  switch(scenario_selected){
    case 0:{//Null scenario
      this->scenario_stop();
      break;
    }

    case 1:{//WP4 scenario
      predManager->start_watcher_prediction();
      captureManager->start_new_capture();
      netManager->start_thread_image();
      gpsManager->start_watcher_gps();
      break;
    }

    case 2://WP5 scenario
    case 3:{
      predManager->start_watcher_prediction();
      captureManager->start_new_capture();
      netManager->start_thread_image();
      gpsManager->start_watcher_gps();
      break;
    }
  }

  //---------------------------
}
