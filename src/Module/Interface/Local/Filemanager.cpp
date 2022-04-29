#include "Filemanager.h"

#include "Saving.h"
#include "Prediction.h"

#include "../Network/GPS.h"
#include "../Interface_node.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../Specific/fct_system.h"
#include "../../../Specific/fct_zenity.h"


//Constructor / Destructor
Filemanager::Filemanager(Interface_node* node){
  this->node_interface = node;
  //---------------------------

  Engine_node* node_engine = node->get_node_engine();
  Configuration* configManager = node_engine->get_configManager();

  this->path_data_dir = configManager->parse_json_s("parameter", "path_data");
  this->is_clean_dir = true;

  //---------------------------
}
Filemanager::~Filemanager(){}

//Subfunctions
void Filemanager::clean_directories(){
  if(is_clean_dir){
    //---------------------------

    GPS* gpsManager = node_interface->get_gpsManager();
    Saving* saveManager = node_interface->get_saveManager();
    Prediction* predManager = node_interface->get_predManager();

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

  GPS* gpsManager = node_interface->get_gpsManager();
  Saving* saveManager = node_interface->get_saveManager();
  Prediction* predManager = node_interface->get_predManager();

  //Get directory paths
  string path_gps = gpsManager->get_path_gps();
  string path_image = saveManager->get_path_image();
  string path_frame = saveManager->get_path_frame();
  string path_predi = predManager->get_path_predi();
  string path_grThr = predManager->get_path_grThr();

  //Clean directories
  create_new_dir(path_data_dir);
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
