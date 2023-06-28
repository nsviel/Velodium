#include "Online.h"
#include "Visibility.h"

#include "../Node_operation.h"

#include "../../Module/Node_module.h"
#include "../../Module/SLAM/Module_slam.h"
#include "../../Module/SLAM/src/SLAM.h"

#include "../../Specific/File/Directory.h"
#include "../../Interface/IO/Recorder.h"
#include "../../Interface/Node_interface.h"
#include "../../Interface/Network/HTTP/http_command.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Color/Color.h"
#include "../../Operation/Transformation/Transformation.h"
#include "../../Operation/Transformation/Filter.h"

#include "../../Engine/Rendering/Renderer.h"
#include "../../Engine/Camera/Followup.h"
#include "../../Engine/Core/Dimension.h"
#include "../../Engine/Core/Engine.h"
#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Scene/Glyph/Object.h"
#include "../../Engine/Core/Configuration.h"

#include "../../Specific/Function/fct_math.h"
#include "../../Specific/Function/fct_transtypage.h"
#include "../../Specific/Function/fct_chrono.h"


//Constructor / Destructor
Online::Online(Node_operation* node_ope){
  //---------------------------

  this->node_engine = node_ope->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();
  this->filterManager = node_ope->get_filterManager();
  this->dimManager = node_engine->get_dimManager();
  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_scene->get_sceneManager();
  this->colorManager = node_ope->get_colorManager();
  this->followManager = node_engine->get_followManager();
  this->objectManager = node_scene->get_objectManager();
  this->renderManager = node_engine->get_renderManager();
  this->visibilityManager = node_ope->get_visibilityManager();
  this->httpManager = new http_command();

  //---------------------------
  this->update_configuration();
}
Online::~Online(){}

//Main function
void Online::update_configuration(){
  //---------------------------

  this->time_ope = 0;
  this->with_subset_specific_color = false;
  this->with_filter_sphere = configManager->parse_json_b("dynamic", "with_filter_sphere");

  //---------------------------
}
void Online::compute_onlineOpe(Collection* collection, int ID_object){
  //This function is called each time a new cloud arrives
  Node_module* node_module = node_engine->get_node_module();
  Object_* object = collection->get_obj_byID(ID_object);
  auto t1 = start_chrono();
  //---------------------------

  //Check for new HTTP command
  this->compute_http_command();

  //Some init operation
  if(object == nullptr) return;
  if(object->obj_type != "cloud") return;
  collection->selected_obj = object;

  //Control object visibilities
  visibilityManager->compute_visibility(collection, ID_object);

  //Make slam on the current object
  if(with_slam){
    node_module->online(collection, ID_object);
    sceneManager->update_buffer_location(object);
  }

  //Make cleaning on the current object
  if(with_filter_sphere){
    filterManager->filter_sphere_cloud((Cloud*)object);
  }

  //If camera follow up option activated
  if(with_camera_follow){
    followManager->camera_followup(collection, ID_object);
  }

  //Colorization
  colorManager->make_colorization(collection, ID_object);

  //Update dynamic interfaces
  this->compute_recording(collection, ID_object);

  //---------------------------
  this->time_ope = stop_chrono(t1);
  this->compute_displayStats(object);
}

//Subfunctions
void Online::compute_recording(Collection* collection, int& ID_object){
  Node_interface* node_interface = node_engine->get_node_interface();
  Recorder* recordManager = node_interface->get_recordManager();
  //---------------------------

  recordManager->compute_online(collection, ID_object);

  //---------------------------
}
void Online::compute_displayStats(Object_* object){
  //---------------------------

  //Consol result
  string stats = object->name + ": ope in ";
  stats += to_string((int)time_ope) + " ms";
  console.AddLog("#", stats);

  //---------------------------
}
void Online::compute_http_command(){
  Node_module* node_module = node_engine->get_node_module();
  Module_slam* module_slam = node_module->get_module_slam();
  SLAM* slamManager = module_slam->get_slamManager();
  //---------------------------

  //Get list of HTTP commands
  vector<vector<string>> option = httpManager->parse_http_config();

  //Parse HTTP commands
  for(int i=0; i<option.size(); i++){
    string opt = option[i][0];
    string val = option[i][1];

    if(opt == "slam"){
      bool* module_with_slam = module_slam->get_with_slam();
      *module_with_slam = string_to_bool(val);
    }
    else if(opt == "view"){
      followManager->camera_mode(val);
    }
    else if(opt == "reset"){
      node_engine->reset();
    }
  }

  //---------------------------
}
