#include "Capture.h"

#include "Scala/Scala.h"
#include "Velodyne/Velodyne.h"

#include "../Module_node.h"

#include "../../Engine/Scene.h"
#include "../../Load/Loader.h"
#include "../../Load/Saver.h"

#include "../../Specific/fct_terminal.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Capture::Capture(Module_node* node_module){
  //---------------------------

  this->onlineManager = node_module->get_onlineManager();
  this->scalaManager = new Scala();
  this->veloManager = new Velodyne();
  this->loaderManager = new Loader();
  this->sceneManager = new Scene();

  this->ID_capture = 0;
  this->with_justOneFrame = true;

  //---------------------------
}
Capture::~Capture(){}

//Main functions
void Capture::start_new_capture(){
  bool* is_rotating = veloManager->get_is_rotating();
  bool* is_connected = veloManager->get_is_connected();
  //---------------------------

  //Check current lidar status
  veloManager->lidar_check_status();

  //If lidar hasn't start, start it
  if(*is_rotating == false){
    veloManager->lidar_start_motor();
  }

  //If all OK start new capture
  if(*is_rotating && *is_connected){
    veloManager->lidar_start_watcher();

    //Reset variables
    int* ID_subset = veloManager->get_ID_subset();
    *ID_subset = 0;

    //Create new empty cloud
    loaderManager->load_cloud_empty();
    cloud_capture = loaderManager->get_createdcloud();
    cloud_capture->name = "Capture_" + to_string(ID_capture);
    ID_capture++;

    console.AddLog("sucess", "Velodyne new capture");
  }else{
    console.AddLog("error", "Problem new capture");
  }

  //---------------------------
}
void Capture::stop_capture(){
  //---------------------------

  //Stop watcher
  bool* is_capturing = veloManager->get_is_capturing();
  *is_capturing = false;

  //Stop lidar motor
  veloManager->lidar_stop_motor();

  //---------------------------
  console.AddLog("#", "Velodyne capture OFF");
}
void Capture::runtime_capturing(){
  //---------------------------

  //Get subset creation flags
  bool* velo_new = veloManager->get_is_newSubset();

  //If flag on, include it in the cloud capture
  if(*velo_new){
    Subset* subset = new Subset(*veloManager->get_subset_capture());

    //Remove all other subset
    if(with_justOneFrame){
      sceneManager->remove_subset_all(cloud_capture);
    }

    //Insert subset data into GPU
    sceneManager->add_subset_to_gpu(subset);

    //Insert the subset inside the capture cloud
    sceneManager->add_new_subset(cloud_capture, subset);

    //Compute online stuff
    onlineManager->compute_onlineOpe(cloud_capture, subset->ID);

    //Unset new Subset flag
    *velo_new = false;
  }

  //---------------------------
}
