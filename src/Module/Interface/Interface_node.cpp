#include "Interface_node.h"

#include "Component/GPS.h"
#include "Component/Saving.h"
#include "Component/Prediction.h"
#include "Component/Network.h"
#include "Component/Filemanager.h"
#include "Component/Watcher.h"

#include "LiDAR/Capture.h"

#include "../Module_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Interface_node::Interface_node(Module_node* node_module){
  //---------------------------

  Engine_node* node_engine = node_module->get_node_engine();

  this->saveManager = new Saving(node_engine);
  this->predManager = new Prediction(node_engine);
  this->captureManager = new Capture(node_module);
  this->gpsManager = new GPS(node_engine);
  this->netManager = new Network();
  this->fileManager = new Filemanager(this);
  this->watcherManager = new Watcher(this, node_engine);

  this->path_dir = get_absolutePath_build() + "/../media/data/capture/";
  this->is_clean_dir = true;

  //---------------------------
  this->init();
}
Interface_node::~Interface_node(){}

//Main functions
void Interface_node::init(){
  //---------------------------

  fileManager->check_directories();

  if(is_clean_dir){
    fileManager->clean_directories();
  }

  watcherManager->watcher_starter();

  //---------------------------
}
void Interface_node::runtime(){
  //---------------------------

  captureManager->runtime_capturing();
  gpsManager->runtime_gps();
  predManager->runtime_prediction();

  //---------------------------
}
