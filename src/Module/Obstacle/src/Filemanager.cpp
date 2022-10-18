#include "Filemanager.h"
#include "Prediction.h"

#include "../Module_obstacle.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../Specific/fct_system.h"
#include "../../../Specific/fct_zenity.h"


//Constructor / Destructor
Filemanager::Filemanager(Module_obstacle* module){
  //---------------------------

  this->configManager = module->get_configManager();
  this->predManager = module->get_predManager();

  //---------------------------
  this->update_configuration();
}
Filemanager::~Filemanager(){}

void Filemanager::update_configuration(){
  //---------------------------

  this->path_data_dir = configManager->parse_json_s("parameter", "path_data");
  this->is_clean_dir = false;

  //---------------------------
}

//Subfunctions
void Filemanager::clean_directories(){
  if(is_clean_dir){
    //---------------------------

    //Get directory paths
    string path_predi = predManager->get_path_predi();
    string path_grThr = predManager->get_path_grThr();

    //Clean directories
    clean_directory_files(path_predi.c_str());
    clean_directory_files(path_grThr.c_str());

    //---------------------------
  }
}
void Filemanager::check_directories(){
  //---------------------------

  //Get directory paths
  string path_predi = predManager->get_path_predi();
  string path_grThr = predManager->get_path_grThr();

  //Clean directories
  create_new_dir(path_data_dir);
  create_new_dir(path_predi);
  create_new_dir(path_grThr);

  //---------------------------
}
void Filemanager::select_dir_path(){
  string path_dir;
  //---------------------------

  zenity_directory(path_dir);

  //---------------------------
}
