#include "Filemanager.h"

#include "GPS.h"
#include "Saving.h"
#include "Prediction.h"

#include "../Interface_node.h"

#include "../../../Specific/fct_system.h"
#include "../../../Specific/fct_zenity.h"


//Constructor / Destructor
Filemanager::Filemanager(Interface_node* node){
  this->node_interface = node;
  //---------------------------

  this->gpsManager = node_interface->get_gpsManager();
  this->saveManager = node_interface->get_saveManager();
  this->predManager = node_interface->get_predManager();

  this->is_clean_dir = true;

  //---------------------------
}
Filemanager::~Filemanager(){}

//Subfunctions
void Filemanager::clean_directories(){
  if(is_clean_dir){
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
}
void Filemanager::check_directories(){
  //---------------------------

  //Get directory paths
  string path_capture = node_interface->get_dir_path();
  string path_gps = gpsManager->get_path_gps();
  string path_image = saveManager->get_path_image();
  string path_frame = saveManager->get_path_frame();
  string path_predi = predManager->get_path_predi();
  string path_grThr = predManager->get_path_grThr();

  //Clean directories
  create_new_dir(path_capture);
  create_new_dir(path_image);
  create_new_dir(path_frame);
  create_new_dir(path_predi);
  create_new_dir(path_grThr);
  create_new_dir(path_gps);

  //---------------------------
}
void Filemanager::select_dir_path(){
  string path_dir;
  //---------------------------

  zenity_directory(path_dir);

  //---------------------------
}
