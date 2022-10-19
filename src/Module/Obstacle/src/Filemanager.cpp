#include "Filemanager.h"

#include "../Module_obstacle.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../Specific/fct_system.h"
#include "../../../Specific/fct_zenity.h"


//Constructor / Destructor
Filemanager::Filemanager(Module_obstacle* module){
  //---------------------------

  this->configManager = module->get_configManager();

  //---------------------------
  this->update_configuration();
}
Filemanager::~Filemanager(){}

void Filemanager::update_configuration(){
  //---------------------------

  this->path_data_dir = configManager->parse_json_s("parameter", "path_data");
  this->path_predi = path_data_dir + "prediction/";
  this->path_grThr = path_data_dir + "groundtruth/";
  this->is_clean_dir = false;

  //---------------------------
}

//Subfunctions
void Filemanager::clean_directories(){
  if(is_clean_dir){
    //---------------------------

    //Clean directories
    clean_directory_files(path_predi.c_str());
    clean_directory_files(path_grThr.c_str());

    //---------------------------
  }
}
void Filemanager::check_directories(){
  //---------------------------

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
