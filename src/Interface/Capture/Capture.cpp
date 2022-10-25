#include "Capture.h"

#include "LiDAR/Scala/Scala.h"
#include "LiDAR/Velodyne/Velodyne.h"

#include "../Node_interface.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Dynamic/Online.h"
#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Load/Processing/Saver.h"

#include "../../Specific/fct_terminal.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Capture::Capture(Node_interface* node_interface){
  //---------------------------

  Node_engine* node_engine = node_interface->get_node_engine();
  Node_load* node_load = node_interface->get_node_load();

  this->configManager = node_engine->get_configManager();
  this->node_ope = node_engine->get_node_ope();
  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loadManager();
  this->scalaManager = new Scala(node_interface);
  this->veloManager = new Velodyne(node_interface);

  //---------------------------
  this->update_configuration();
}
Capture::~Capture(){}

//Main functions
void Capture::update_configuration(){
  //---------------------------

  this->lidar_model = configManager->parse_json_s("interface", "lidar_model");
  this->capture_port = configManager->parse_json_i("interface", "capture_port");
  this->cloud_capture = nullptr;
  this->ID_capture = 0;
  this->is_capturing = false;
  this->is_capture_finished = true;
  this->capture_time = 0;
  this->capture_nb_point = 0;
  this->capture_nb_point_raw = 0;
  this->is_first_run = true;
  this->with_justOneFrame = false;
  this->nb_subset_max = 50;

  bool with_capture = configManager->parse_json_i("interface", "with_capture");
  if(with_capture){
    this->start_new_capture(lidar_model);
  }

  //---------------------------
}
void Capture::runtime(){
  //---------------------------

  if(is_capturing == false){return;}
  if(cloud_capture == nullptr){return;}
  if(sceneManager->get_is_list_empty()){
    this->is_capturing = false;
    this->is_capture_finished = true;
    return;
  }

  //Check for new Subset
  if(lidar_model == "velodyne_vlp16"){
    this->runtime_velodyne();
  }
  else if(lidar_model == "scala"){
    this->runtime_scala();
  }

  //---------------------------
}

//Capture functions
void Capture::start_new_capture(string model){
  //---------------------------

  if(is_capture_finished == true && is_capturing == false){
    //Default : vlp16
    if(model == "" || model == "velodyne_vlp16"){
      this->capture_vlp16();
      this->lidar_model = "velodyne_vlp16";
      this->is_capture_finished = false;
    }
    else if(model == "scala"){
      this->capture_scala();
      this->lidar_model = "scala";
      this->is_capture_finished = false;
    }
    else{
      this->is_capture_finished = true;
      string log = "Could not capture " + model + " LiDAR";
      console.AddLog("error", log);
    }
  }else if(is_capture_finished == false && is_capturing == false){
    this->is_capturing = true;
  }

  //---------------------------
}
void Capture::stop_capture(){
  //---------------------------

  if(lidar_model == "velodyne_vlp16"){
    *veloManager->get_is_capturing() = false;
  }
  else if(lidar_model == "scala"){
    *scalaManager->get_is_scala_capturing() = false;
  }

  this->is_capturing = false;
  this->is_capture_finished = true;

  //---------------------------
  console.AddLog("#", "Watcher capture OFF");
}

//LiDAR specific functions
void Capture::runtime_velodyne(){
  Subset* new_subset = nullptr;
  //---------------------------

  bool *new_capture = veloManager->get_is_newSubset();

  if(*new_capture){
    //Pick new subset
    new_subset = veloManager->get_subset_capture();

    //Unset new Subset flag
    *new_capture = false;
    this->capture_time = toc();tic();
    this->capture_nb_point_raw = new_subset->xyz.size();
  }

  //If new subset, include it in the capture cloud
  if(new_subset != nullptr){
    //Make new subset stuff
    this->operation_new_subset(new_subset);
  }

  //---------------------------
}
void Capture::runtime_scala(){
  Subset* new_subset = nullptr;
  //---------------------------

  bool* new_capture = scalaManager->get_is_newSubset();
  if(*new_capture){
    //Pick new subset
    new_subset = new Subset(*scalaManager->get_subset_capture());

    //Unset new Subset flag
    *new_capture = false;
  }

  //If new subset, include it in the capture cloud
  if(new_subset != nullptr){
    //Make new subset stuff
    this->operation_new_subset(new_subset);
  }

  //---------------------------
}
void Capture::capture_vlp16(){
  bool is_capturing = *veloManager->get_is_capturing();
  //---------------------------

  if(is_capturing == false){
    veloManager->lidar_start_watcher(capture_port);
  }

  //Create new empty cloud
  loaderManager->load_cloud_empty();
  cloud_capture = loaderManager->get_createdcloud();
  cloud_capture->ID_subset = 0;
  cloud_capture->name = "Capture_vlp16_" + to_string(ID_capture);

  this->is_capturing = true;
  ID_capture++;

  //Regulate the number of subsets
  this->control_nb_subset(cloud_capture);

  //---------------------------
  console.AddLog("ok", "Watcher - Vlp16 capture");
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
  console.AddLog("ok", "Watcher - Scala capture");
}

//Subfunctions
void Capture::operation_new_subset(Subset* subset){
  //---------------------------

  //Do not record the first run
  if(is_first_run){
    sceneManager->remove_subset_last(cloud_capture);
    cloud_capture->nb_subset = 0;
    is_first_run = false;
  }

  //Set new subset identifieurs
  subset->name = "frame_" + to_string(cloud_capture->ID_subset);
  subset->ID = cloud_capture->ID_subset;
  cloud_capture->ID_subset++;

  //Supress null points
  this->supress_nullpoints(subset);
  this->capture_nb_point = subset->xyz.size();

  //If ok insert subset into scene
  if(subset->xyz.size() != 0){
    //Update subset data
    sceneManager->update_subset_location(subset);

    //Insert the subset inside the capture cloud
    sceneManager->add_new_subset(cloud_capture, subset);

    //Compute online stuff
    Online* onlineManager = node_ope->get_onlineManager();
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
void Capture::control_nb_subset(Cloud* cloud){
  //---------------------------

  //If option, remove all other subset
  if(with_justOneFrame){
    sceneManager->remove_subset_last(cloud);
  }
  //Remove old frame if option is activated
  else{
    if(cloud->subset.size() > nb_subset_max){
      sceneManager->remove_subset_last(cloud);
    }
  }

  //---------------------------
}
