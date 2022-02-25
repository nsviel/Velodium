#include "Interface.h"

#include "Component/GPS.h"
#include "Component/Saving.h"
#include "Component/Prediction.h"
#include "Component/Network.h"
#include "Component/Filemanager.h"

#include "LiDAR/Capture.h"

#include "../Module_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Load/Pather.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Interface::Interface(Module_node* node_module){
  //---------------------------

  Engine_node* node_engine = node_module->get_node_engine();

  this->saveManager = new Saving(node_engine->get_renderManager());
  this->gpsManager = new GPS();
  this->predManager = new Prediction();
  this->netManager = new Network();
  this->captureManager = new Capture(node_module);
  this->fileManager = new Filemanager(this);

  this->path_dir = get_absolutePath_build() + "/../media/data/capture/";
  this->is_clean_dir = true;

  //---------------------------
  this->init();
}
Interface::~Interface(){}

//Main functions
void Interface::init(){
  //---------------------------

  fileManager->check_directories();

  if(is_clean_dir){
    fileManager->clean_directories();
  }

  //---------------------------
}
void Interface::runtime(){
  //---------------------------

  captureManager->runtime_capturing();
  gpsManager->runtime_gps();
  predManager->runtime_prediction();

  //---------------------------
}
