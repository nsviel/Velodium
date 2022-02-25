#include "Capture.h"

#include "Scala/Scala.h"
#include "Velodyne/Velodyne.h"

#include "../../Module_node.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Load/Load_node.h"
#include "../../../Load/Processing/Loader.h"
#include "../../../Load/Processing/Saver.h"

#include "../../../Specific/fct_terminal.h"
#include "../../../Specific/fct_system.h"


//Constructor / Destructor
Capture::Capture(Module_node* node_module){
  //---------------------------

  Engine_node* node_engine = node_module->get_node_engine();
  Load_node* node_load = node_engine->get_node_load();

  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loadManager();
  this->scalaManager = new Scala(node_engine);
  this->veloManager = new Velodyne(node_engine);

  this->ID_capture = 0;
  this->nb_subset_max = 20;
  this->with_justOneFrame = false;
  this->is_capturing = false;

  //---------------------------
}
Capture::~Capture(){}

//Main functions
void Capture::start_new_capture(){
  bool* is_rotating = veloManager->get_is_rotating();
  bool* is_connected = veloManager->get_is_connected();
  //---------------------------

  /*//Check current lidar status
  veloManager->lidar_check_status();

  //If lidar hasn't start, start it
  if(*is_rotating == false){
    veloManager->lidar_start_motor();
  }*/

  //If all OK start new capture
  //if(*is_rotating && *is_connected){
  if(true){
    veloManager->lidar_start_watcher();

    //Create new empty cloud
    loaderManager->load_cloud_empty();
    cloud_capture = loaderManager->get_createdcloud();
    cloud_capture->name = "Capture_" + to_string(ID_capture);

    this->is_capturing = true;
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
  bool* is_capturing = veloManager->get_is_velo_capturing();
  *is_capturing = false;
  this->is_capturing = false;

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
    this->operation_new_subset(subset);

    //Unset new Subset flag
    *velo_new = false;
  }

  //---------------------------
}

//Subfunctions
void Capture::operation_new_subset(Subset* subset){
  //---------------------------

  subset->name = "frame_" + to_string(cloud_capture->ID_subset);
  subset->ID = cloud_capture->ID_subset;
  cloud_capture->ID_subset++;

  //If option, remove all other subset
  if(with_justOneFrame){
    sceneManager->remove_subset_all(cloud_capture);
  }
  //Remove old frame if option is activated
  else{
    if(cloud_capture->subset.size() >= nb_subset_max){
      sceneManager->remove_subset_last(cloud_capture);
    }
  }

  //Supress null points
  this->supress_nullpoints(subset);

  sayHello();

  //FUITE DE MEMOIRE ICIIIIIIIIIIIIIIIIIIII


  //If ok insert subset into scene
  if(subset->xyz.size() != 0){
    //Insert subset data into GPU
    sceneManager->add_subset_to_gpu(subset);

    //Insert the subset inside the capture cloud
    sceneManager->add_new_subset(cloud_capture, subset);
say("end");
    //Compute online stuff
    //onlineManager->compute_onlineOpe(cloud_capture, subset->ID);
  }

  delete subset;

  //---------------------------
}
void Capture::supress_nullpoints(Subset* subset){
  vector<vec3> xyz;
  vector<vec4> RGB;
  vector<vec3> N;
  vector<float> I;
  vector<float> ts;
  //---------------------------

  for(int i=0; i<subset->xyz.size(); i++){
    if(subset->xyz[i].x != 0 && subset->xyz[i].y != 0 && subset->xyz[i].z != 0){
      xyz.push_back(subset->xyz[i]);

      if(subset->RGB.size() != 0){
        RGB.push_back(subset->RGB[i]);
      }

      if(subset->N.size() != 0){
        N.push_back(subset->N[i]);
      }

      if(subset->ts.size() != 0){
        ts.push_back(subset->ts[i]);
      }

      if(subset->I.size() != 0){
        I.push_back(subset->I[i]);
      }
    }
  }

  subset->xyz = xyz;
  if(RGB.size() != 0){
    subset->RGB = RGB;
  }
  if(N.size() != 0){
    subset->N = N;
  }
  if(I.size() != 0){
    subset->I = I;
  }
  if(ts.size() != 0){
    subset->ts = ts;
  }

  //---------------------------
}
