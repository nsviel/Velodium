#include "Capture.h"

#include "Scala/Scala.h"
#include "Velodyne/Velodyne.h"

#include "../../Engine/Scene.h"
#include "../../Load/Loader.h"
#include "../../Load/Saver.h"
#include "../../Load/Processing/dataExtraction.h"

#include "../../Specific/fct_display.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Capture::Capture(){
  //---------------------------

  this->scalaManager = new Scala();
  this->veloManager = new Velodyne();
  this->loaderManager = new Loader();
  this->extractManager = new dataExtraction();
  this->sceneManager = new Scene();

  this->ID_capture = 0;
  this->with_justOneFrame = false;

  //---------------------------
}
Capture::~Capture(){}

//Main functions
void Capture::init_new_capture(){
  //---------------------------

  //Reset variables
  this->ID_subset = 0;

  //Create new empty cloud
  loaderManager->load_cloud_empty();
  cloud_capture = loaderManager->get_createdcloud();
  cloud_capture->name = "Capture_" + to_string(ID_capture);

  //---------------------------
  ID_capture++;
}
void Capture::runtime_check_newSubset(){
  //---------------------------

  //Get subset creation flags
  bool* velo_new = veloManager->get_is_newSubset();

  //If flag on, include it in the cloud capture
  if(*velo_new){
    Subset* subset = veloManager->get_subset_capture();

    //Remove all other subset
    if(with_justOneFrame){
      sceneManager->remove_subset_all(cloud_capture);
    }

    //Insert the subset inside the capture cloud
    sceneManager->add_new_subset(cloud_capture, subset);

    //Unset new Subset flag
    *velo_new = false;
  }

  //---------------------------
}
