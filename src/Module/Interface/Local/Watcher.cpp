#include "Watcher.h"

#include "GPS.h"
#include "Prediction.h"

#include "../LiDAR/Capture.h"
#include "../Interface_node.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Configuration.h"


//Constructor / Destructor
Watcher::Watcher(Interface_node* node_interface, Engine_node* node_engine){
  //---------------------------

  this->configManager = node_engine->get_configManager();
  this->gpsManager = node_interface->get_gpsManager();
  this->captureManager = node_interface->get_captureManager();
  this->predManager = node_interface->get_predManager();

  //---------------------------
}
Watcher::~Watcher(){}

void Watcher::watcher_starter(){
  //---------------------------

  //Prediction watcher
  bool watcher_pred = configManager->parse_json_b("watcher", "prediction");
  if(watcher_pred){
    predManager->start_watcher_prediction();
  }

  //GPS watcher
  bool watcher_gps = configManager->parse_json_b("watcher", "gps");
  if(watcher_gps){
    gpsManager->start_watcher_gps();
  }

  //capture watcher
  bool watcher_capture = configManager->parse_json_b("watcher", "capture");
  if(watcher_capture){
    captureManager->start_new_capture();
  }

  //---------------------------
}
