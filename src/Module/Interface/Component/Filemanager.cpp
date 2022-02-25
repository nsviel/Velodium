#include "Filemanager.h"

#include "GPS.h"
#include "Saving.h"
#include "Prediction.h"

#include "../Interface.h"

#include "../../../Load/Pather.h"
#include "../../../Specific/fct_system.h"


//Constructor / Destructor
Filemanager::Filemanager(Interface* ioManager){
  //---------------------------

  this->gpsManager = ioManager->get_gpsManager();
  this->saveManager = ioManager->get_saveManager();
  this->predManager = ioManager->get_predManager();

  //---------------------------
}
Filemanager::~Filemanager(){}

//Subfunctions
void Filemanager::clean_directories(){
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
void Filemanager::check_directories(){
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
void Filemanager::select_dir_path(){
  string path_dir;
  //---------------------------

  Pather pathManager;
  pathManager.zenity_select_directory(path_dir);

  //---------------------------
}
