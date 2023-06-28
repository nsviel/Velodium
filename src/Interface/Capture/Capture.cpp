#include "Capture.h"

#include "Scala/Scala.h"
#include "Velodyne/Velodyne.h"

#include "../Node_interface.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Dynamic/Online.h"
#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Engine/Core/Configuration.h"
#include "../../Engine/GPU/GPU_data.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Load/Processing/Saver.h"

#include "../../Specific/Function/fct_terminal.h"
#include "../../Specific/File/Directory.h"


//Constructor / Destructor
Capture::Capture(Node_interface* node_interface){
  //---------------------------

  Node_engine* node_engine = node_interface->get_node_engine();
  Node_load* node_load = node_interface->get_node_load();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->configManager = node_engine->get_configManager();
  this->node_ope = node_engine->get_node_ope();
  this->sceneManager = node_scene->get_sceneManager();
  this->loaderManager = node_load->get_loaderManager();
  this->scalaManager = new Scala(node_interface);
  this->veloManager = new Velodyne(node_interface);
  this->gpuManager = new GPU_data();

  this->point_size = 1;

  //---------------------------
  this->update_configuration();
}
Capture::~Capture(){}

//Main functions
void Capture::update_configuration(){
  //---------------------------

  this->lidar_model = configManager->parse_json_s("interface", "lidar_model");
  this->capture_port = configManager->parse_json_i("interface", "capture_port");
  this->ratio_frame = configManager->parse_json_i("interface", "ratio_frame");
  this->ratio_cpt = ratio_frame;

  this->collection_capture = nullptr;
  this->ID_capture = 0;
  this->capture_nb_point = 0;
  this->capture_nb_point_raw = 0;
  this->nb_subset_max = 50;

  this->with_supress_null = false;
  this->with_justOneFrame = false;
  this->is_first_run = true;
  this->is_capturing = false;
  this->is_capture_finished = true;

  if(configManager->parse_json_b("interface", "with_capture")){
    this->start_new_capture(lidar_model);
  }

  //---------------------------
}
void Capture::start_new_capture(string model){
  //---------------------------

  if(is_capture_finished == true && is_capturing == false){
    //Default : vlp16
    if(model == "velodyne_vlp16"){
      this->start_capture_velodyne();
      this->lidar_model = "velodyne_vlp16";
      this->is_capture_finished = false;
    }
    else if(model == "scala"){
      this->start_capture_scala();
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
    veloManager->stop_watcher();
  }
  else if(lidar_model == "scala"){
    scalaManager->stop_watcher();
  }

  this->is_capturing = false;
  this->is_capture_finished = true;

  //---------------------------
  console.AddLog("#", "Watcher capture OFF");
}

//Runtime function
void Capture::runtime(){
  //---------------------------

  if(is_capturing == false){
    return;
  }

  //Check if capture cloud is always here
  if(sceneManager->get_is_list_empty() || collection_capture == nullptr){
    this->stop_capture();
    return;
  }

  //Check for new Cloud
  if(lidar_model == "velodyne_vlp16"){
    this->runtime_velodyne();
  }
  else if(lidar_model == "scala"){
    this->runtime_scala();
  }

  //Regulate the number of subsets
  this->control_nb_subset(collection_capture);

  //---------------------------
}
void Capture::runtime_velodyne(){
  Cloud* new_subset = nullptr;
  //---------------------------

  bool* new_capture = veloManager->get_is_newSubset();

  if(*new_capture){
    //Pick new cloud
    new_subset = veloManager->get_obj_capture();

    //Unset new Cloud flag
    *new_capture = false;
    this->capture_nb_point_raw = new_subset->xyz.size();

    //If new cloud, make new cloud stuff
    if(new_subset != nullptr){
      this->operation_new_subset(new_subset);
    }
  }

  //---------------------------
}
void Capture::runtime_scala(){
  Cloud* new_subset = nullptr;
  //---------------------------

  bool* new_capture = scalaManager->get_is_newSubset();
  if(*new_capture){
    //Pick new cloud
    new_subset = new Cloud(*scalaManager->get_obj_capture());

    //Unset new Cloud flag
    *new_capture = false;
  }

  //If new cloud, include it in the capture cloud
  if(new_subset != nullptr){
    //Make new cloud stuff
    this->operation_new_subset(new_subset);
  }

  //---------------------------
}

//LiDAR specific functions
void Capture::start_capture_velodyne(){
  bool is_capturing = *veloManager->get_run_capture();
  //---------------------------

  if(is_capturing == false){
    veloManager->start_watcher(capture_port);
    this->is_capturing = true;
  }

  //Create new empty cloud
  this->create_empty_cloud();

  //---------------------------
  console.AddLog("ok", "Watcher - Vlp16 capture");
}
void Capture::start_capture_scala(){
  //---------------------------

  scalaManager->start_watcher();

  //Create new empty cloud
  this->collection_capture = loaderManager->load_cloud_empty();
  collection_capture->name = "start_capture_scala_" + to_string(ID_capture);

  this->is_capturing = true;
  ID_capture++;

  //---------------------------
  console.AddLog("ok", "Watcher - Scala capture");
}

//Subfunctions
void Capture::operation_new_subset(Cloud* cloud){
  //---------------------------

  //We take only one frame amounst several with ratio_frame
  int modulo = ratio_frame - ratio_cpt;
  ratio_cpt++;

  //If ok insert cloud into scene
  if(modulo == 0){
    ratio_cpt = 1;

    //Do not record the first run
    if(is_first_run){
      collection_capture->obj_remove_last();
      collection_capture->nb_obj = 0;
      is_first_run = false;
    }

    //Supress null points
    if(with_supress_null){
      this->supress_nullpoints(cloud);
      this->capture_nb_point = cloud->xyz.size();
      if(cloud->xyz.size() == 0) return;
    }

    //Set new cloud identifieurs
    cloud->name = "frame_" + to_string(collection_capture->ID_obj_last);
    cloud->ID = collection_capture->ID_obj_last;
    cloud->draw_point_size = point_size;
    collection_capture->ID_obj_last++;

    //Update cloud data
    sceneManager->update_buffer_location(cloud);

    //Insert the cloud inside the capture cloud
    collection_capture->obj_add_new(cloud);

    //Compute online stuff
    Online* onlineManager = node_ope->get_onlineManager();
    onlineManager->compute_onlineOpe(collection_capture, cloud->ID);
  }

  //---------------------------
}
void Capture::supress_nullpoints(Cloud* cloud){
  vector<vec3> xyz;
  vector<vec4> RGB;
  vector<vec3> N;
  vector<float> I;
  vector<float> ts;
  //---------------------------

  for(int i=0; i<cloud->xyz.size(); i++){
    if(cloud->xyz[i].x != 0 && cloud->xyz[i].y != 0 && cloud->xyz[i].z != 0){
      //Location
      xyz.push_back(cloud->xyz[i]);

      //Color
      if(cloud->rgb.size() != 0){
        RGB.push_back(cloud->rgb[i]);
      }

      //Normal
      if(cloud->Nxyz.size() != 0){
        N.push_back(cloud->Nxyz[i]);
      }

      //Timestamp
      if(cloud->ts.size() != 0){
        ts.push_back(cloud->ts[i]);
      }

      //Intensity
      if(cloud->I.size() != 0){
        I.push_back(cloud->I[i]);
      }
    }
  }

  cloud->xyz = xyz;
  if(RGB.size() != 0){
    cloud->rgb = RGB;
  }
  if(N.size() != 0){
    cloud->Nxyz = N;
  }
  if(I.size() != 0){
    cloud->I = I;
  }
  if(ts.size() != 0){
    cloud->ts = ts;
  }

  //---------------------------
}
void Capture::control_nb_subset(Collection* collection){
  //---------------------------

  //If option, remove all other cloud
  if(with_justOneFrame){
    collection->obj_remove_last();
  }
  //Remove old frame if option is activated
  else{
    if(collection->list_obj.size() > nb_subset_max){
      collection->obj_remove_last();
    }
  }

  //---------------------------
}
void Capture::create_empty_cloud(){
  //---------------------------

  this->collection_capture = loaderManager->load_cloud_empty();
  this->collection_capture->ID_obj_last = 0;
  this->collection_capture->name = "Capture_" + to_string(ID_capture);
  this->ID_capture++;

  //---------------------------
}
