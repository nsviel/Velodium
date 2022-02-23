#include "Interface.h"

#include "Component/GPS.h"
#include "Component/Saving.h"
#include "Component/Prediction.h"
#include "Component/Network.h"

#include "LiDAR/Capture.h"

#include "../Module_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Load/Pather.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Interface::Interface(Module_node* node_module){
  //---------------------------

  Engine_node* node_engine = node_module->get_node_engine();

  this->gpsManager = new GPS();
  this->saveManager = new Saving(node_engine->get_renderManager());
  this->predManager = new Prediction();
  this->netManager = new Network();
  this->captureManager = new Capture(node_module);

  this->path_dir = get_absolutePath_build() + "/../media/data/capture/";
  this->is_clean_dir = true;

  //---------------------------
  this->init();
}
Interface::~Interface(){}

//Main functions
void Interface::init(){
  //---------------------------

  this->check_directories();

  if(is_clean_dir){
    this->clean_directories();
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

//Subfunctions
void Interface::clean_directories(){
  //---------------------------

  //Get directory paths
  string path_gps = gpsManager->get_path_gps();
  string path_image = saveManager->get_path_image();
  string path_frame = saveManager->get_path_frame();
  string path_predi = predManager->get_path_predi();
  string path_grThr = predManager->get_path_grThr();

  //Clean directories
  clean_directory_files(path_image.c_str());
  clean_directory_files(path_frame.c_str());
  clean_directory_files(path_predi.c_str());
  clean_directory_files(path_grThr.c_str());
  clean_directory_files(path_gps.c_str());

  //---------------------------
}
void Interface::check_directories(){
  //---------------------------

  //Get directory paths
  string path_gps = gpsManager->get_path_gps();
  string path_image = saveManager->get_path_image();
  string path_frame = saveManager->get_path_frame();
  string path_predi = predManager->get_path_predi();
  string path_grThr = predManager->get_path_grThr();

  //Clean directories
  clean_directory_files(path_image.c_str());
  clean_directory_files(path_frame.c_str());
  clean_directory_files(path_predi.c_str());
  clean_directory_files(path_grThr.c_str());
  clean_directory_files(path_gps.c_str());

  //---------------------------
}
void Interface::select_dir_path(){
  //---------------------------

  Pather pathManager;
  pathManager.zenity_select_directory(path_dir);

  //---------------------------
}
