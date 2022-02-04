#include "Capture.h"

#include "Scala/Scala.h"
#include "Velodyne/Velodyne.h"

#include "../../Engine/Scene.h"
#include "../../Load/Loader.h"
#include "../../Load/Saver.h"

#include "../../Specific/fct_display.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Capture::Capture(){
  //---------------------------

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
  //---------------------------

  bool is_rotating = *veloManager->get_is_rotating();
  bool is_connected = *veloManager->get_is_connected();

  //If lidar hasn't start, start it
  if(is_rotating == false){
    veloManager->lidar_start_motor();
  }

  if(is_rotating && is_connected){
    veloManager->lidar_start_watcher();

    //Reset variables
    int* ID_subset = veloManager->get_ID_subset();
    *ID_subset = 0;

    //Create new empty cloud
    loaderManager->load_cloud_empty();
    cloud_capture = loaderManager->get_createdcloud();
    cloud_capture->name = "Capture_" + to_string(ID_capture);
    ID_capture++;
  }else{
    return;
  }

  //---------------------------
  console.AddLog("#", "Velodyne new capture");
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

    //Unset new Subset flag
    *velo_new = false;
  }

  //---------------------------
}
