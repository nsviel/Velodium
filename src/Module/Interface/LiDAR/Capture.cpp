#include "Capture.h"

#include "Scala/Scala.h"
#include "Velodyne/Velodyne.h"

#include "../Interface_node.h"

#include "../../Module_node.h"
#include "../../Player/Player_node.h"
#include "../../Player/Dynamic/Online.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Configuration.h"
#include "../../../Load/Load_node.h"
#include "../../../Load/Processing/Loader.h"
#include "../../../Load/Processing/Saver.h"

#include "../../../Specific/fct_terminal.h"
#include "../../../Specific/fct_system.h"


//Constructor / Destructor
Capture::Capture(Interface_node* node){
  //---------------------------

  Engine_node* node_engine = node->get_node_engine();
  Load_node* node_load = node_engine->get_node_load();
  Configuration* configManager = node_engine->get_configManager();

  this->node_module = node->get_node_module();
  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loadManager();
  this->scalaManager = new Scala(node);
  this->veloManager = new Velodyne(node_engine);

  this->lidar_model = configManager->parse_json_s("interface", "lidar_model");
  this->ID_capture = 0;
  this->is_capturing = false;

  //---------------------------
}
Capture::~Capture(){}

//Main functions
void Capture::start_new_capture(){
  //---------------------------

  if(lidar_model == "velodyne_vlp16"){
    this->capture_vlp16();
  }
  else if(lidar_model == "scala"){
    this->capture_scala();
  }

  //---------------------------
}
void Capture::stop_capture(){
  //---------------------------

  if(lidar_model == "velodyne_vlp16"){
    *veloManager->get_is_velo_capturing() = false;
  }
  else if(lidar_model == "scala"){
    *scalaManager->get_is_scala_capturing() = false;
  }

  this->is_capturing = false;

  //---------------------------
  console.AddLog("#", "Watcher capture OFF");
}
void Capture::runtime_capturing(){
  //---------------------------

  //Initiate
  Subset* new_subset;
  new_subset = nullptr;

  //Check for new Subset
  if(lidar_model == "velodyne_vlp16"){
    bool *new_capture = veloManager->get_is_newSubset();

    if(*new_capture){
      //Pick new subset
      new_subset = veloManager->get_subset_capture();

      //Unset new Subset flag
      *new_capture = false;
    }
  }
  else if(lidar_model == "scala"){
    bool* new_capture = scalaManager->get_is_newSubset();
    if(*new_capture){
      //Pick new subset
      new_subset = new Subset(*scalaManager->get_subset_capture());

      //Unset new Subset flag
      *new_capture = false;
    }
  }

  //If new subset, include it in the capture cloud
  if(new_subset != nullptr){
    //Make new subset stuff
    this->operation_new_subset(new_subset);
  }

  //---------------------------
}

//LiDAR specific functions
void Capture::capture_vlp16(){
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
    cloud_capture->ID_subset = 0;
    cloud_capture->name = "Capture_vlp16_" + to_string(ID_capture);

    this->is_capturing = true;
    ID_capture++;
  }else{
    console.AddLog("error", "Problem new Vlp16 capture");
    return;
  }

  //---------------------------
  console.AddLog("success", "Watcher - Vlp16 capture");
}
void Capture::capture_scala(){
  //---------------------------

  scalaManager->lidar_start_watcher();

  //Create new empty cloud
  loaderManager->load_cloud_empty();
  cloud_capture = loaderManager->get_createdcloud();
  cloud_capture->name = "Capture_scala_" + to_string(ID_capture);

  this->is_capturing = true;
  ID_capture++;

  //---------------------------
  console.AddLog("success", "Watcher - Scala capture");
}

//Subfunctions
void Capture::operation_new_subset(Subset* subset){
  //---------------------------

  //Set new subset identifieurs
  subset->name = "frame_" + to_string(cloud_capture->ID_subset);
  subset->ID = cloud_capture->ID_subset;
  cloud_capture->ID_subset++;

  //Supress null points
  this->supress_nullpoints(subset);

  //If ok insert subset into scene
  if(subset->xyz.size() != 0){
    //Update subset data
    sceneManager->update_subset_location(subset);

    //Insert the subset inside the capture cloud
    sceneManager->add_new_subset(cloud_capture, subset);

    //Compute online stuff
    Player_node* node_player = node_module->get_node_player();
    Online* onlineManager = node_player->get_onlineManager();
    onlineManager->compute_onlineOpe(cloud_capture, subset->ID);
  }

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
