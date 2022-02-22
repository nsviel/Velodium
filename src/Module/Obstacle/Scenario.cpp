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

  this->scenario_selected = 0;
  this->scenario_started = false;

  //---------------------------
}
Scenario::~Scenario(){}

void Scenario::scenario_start(){
  //---------------------------

  switch(scenario_selected){
    case 0:{//WP4 auto
      this->scenario_WP4_auto();
      break;
    }
    case 1:{//WP5 train on-board
      this->scenario_WP5_train_board();
      break;
    }
    case 2:{//WP5 train on-edge
      this->scenario_WP5_train_edge();
      break;
    }
  }

  //---------------------------
}
void Scenario::scenario_stop(){
  //---------------------------


  //---------------------------
}

void Scenario::scenario_WP4_auto(){
  //---------------------------

  slam_configManager->set_predefined_conf(1);
  this->make_configuration();

  //Start runtime stuff
  this->make_watcher();

  //---------------------------
}
void Scenario::scenario_WP5_train_board(){
  //---------------------------

  //Parameters
  slam_configManager->set_predefined_conf(1);
  this->make_configuration();

  //Start runtime stuff
  this->make_watcher();

  //---------------------------
}
void Scenario::scenario_WP5_train_edge(){
  //---------------------------

  //Parameters
  slam_configManager->set_predefined_conf(1);
  this->make_configuration();

  //Start runtime stuff
  this->make_watcher();

  //---------------------------
}

void Scenario::make_configuration(){
  //---------------------------

  //Parameters
  bool* with_camera = onlineManager->get_with_camera_follow();
  bool* with_slam = onlineManager->get_with_slam();
  bool* with_cylinder = onlineManager->get_with_cylinder_filter();
  bool* with_heatmap = onlineManager->get_with_heatmap();
  bool* with_save_frame = onlineManager->get_with_save_frame();
  bool* with_save_image = onlineManager->get_with_save_image();

  switch(scenario_selected){
    case 0:{
      *with_camera = true;
      *with_slam = true;
      *with_cylinder = true;
      *with_heatmap = true;
      *with_save_frame = true;
      *with_save_image = true;
      break;
    }

    case 1:
    case 2:{
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

  Prediction* predManager = ioManager->get_predManager();
  GPS* gpsManager = ioManager->get_gpsManager();

  predManager->start_watcher_prediction();
  captureManager->start_new_capture();
  netManager->start_thread_image();
  gpsManager->start_watcher_gps();

  bool is_capture_watcher = captureManager->get_is_capture_watcher();
  bool is_pred_watcher = predManager->get_is_watching();
  bool is_image_watcher = netManager->get_is_image_watcher();

  if(!is_capture_watcher || !is_pred_watcher || !is_image_watcher){
    console.AddLog("error", "Probleme with scenario element");
  }

  //---------------------------
}
