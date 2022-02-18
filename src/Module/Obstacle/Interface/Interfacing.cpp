#include "Interfacing.h"

#include "GPS.h"
#include "Saving.h"
#include "Prediction.h"

#include "../../Module_node.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Load/Pather.h"
#include "../../../Specific/fct_system.h"


//Constructor / Destructor
Interfacing::Interfacing(Module_node* node_module){
  //---------------------------

  Engine_node* node_engine = node_module->get_node_engine();

  this->gpsManager = new GPS();
  this->saveManager = new Saving(node_engine->get_renderManager());
  this->predManager = new Prediction();

  this->path_dir = get_absolutePath_build() + "/../media/data/capture/";

  //---------------------------
  this->clean_directories();
}
Interfacing::~Interfacing(){}

//Main functions
void Interfacing::runtime(){
  //---------------------------

  gpsManager->runtime_gps();
  predManager->runtime_prediction();

  //---------------------------
}

//Subfunctions
void Interfacing::clean_directories(){
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
void Interfacing::select_dir_path(){
  //---------------------------

  Pather pathManager;
  pathManager.zenity_select_directory(path_dir);

  //---------------------------
}
