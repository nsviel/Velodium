#include "Interface_node.h"

#include "Local/GPS.h"
#include "Local/Saving.h"
#include "Local/Prediction.h"
#include "Network/Network.h"
#include "Local/Filemanager.h"

#include "LiDAR/Capture.h"

#include "../Module_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Interface_node::Interface_node(Module_node* node_module){
  //---------------------------

  this->node_engine = node_module->get_node_engine();

  this->fileManager = new Filemanager(this);
  this->saveManager = new Saving(this);
  this->predManager = new Prediction(node_engine);
  this->captureManager = new Capture(node_module);
  this->gpsManager = new GPS(node_engine);
  this->netManager = new Network();

  //---------------------------
  this->init();
}
Interface_node::~Interface_node(){}

//Main functions
void Interface_node::init(){
  //---------------------------

  fileManager->check_directories();
  fileManager->clean_directories();

  //---------------------------
}
void Interface_node::runtime(){
  //---------------------------

  captureManager->runtime_capturing();
  gpsManager->runtime_gps();
  predManager->runtime_prediction();

  //---------------------------
}
