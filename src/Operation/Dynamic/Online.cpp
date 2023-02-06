#include "Online.h"
#include "Visibility.h"

#include "../Node_operation.h"

#include "../../Module/Node_module.h"
#include "../../Module/SLAM/Module_slam.h"
#include "../../Module/SLAM/src/SLAM.h"

#include "../../Interface/File/Directory.h"
#include "../../Interface/IO/Recorder.h"
#include "../../Interface/Node_interface.h"
#include "../../Interface/Network/HTTP/http_command.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Color/Color.h"
#include "../../Operation/Transformation/Transformation.h"
#include "../../Operation/Transformation/Filter.h"

#include "../../Engine/OpenGL/Renderer.h"
#include "../../Engine/Camera/Followup.h"
#include "../../Engine/Core/Dimension.h"
#include "../../Engine/Core/Engine.h"
#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Glyph/Object.h"
#include "../../Engine/Scene/Configuration.h"

#include "../../Specific/fct_math.h"
#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_chrono.h"


//Constructor / Destructor
Online::Online(Node_operation* node_ope){
  //---------------------------

  this->node_engine = node_ope->get_node_engine();
  this->filterManager = node_ope->get_filterManager();
  this->dimManager = node_engine->get_dimManager();
  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->colorManager = node_ope->get_colorManager();
  this->followManager = node_engine->get_followManager();
  this->objectManager = node_engine->get_objectManager();
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
void Online::compute_onlineOpe(Cloud* cloud, int ID_subset){
  //This function is called each time a new subset arrives
  Node_module* node_module = node_engine->get_node_module();
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  auto t1 = start_chrono();
  //---------------------------

  //Check for new HTTP command
  this->compute_http_command();

  //Some init operation
  if(subset == nullptr) return;
  cloud->subset_selected = subset;

  //Control subset visibilities
  visibilityManager->compute_visibility(cloud, ID_subset);

  //Make slam on the current subset
  node_module->online(cloud, ID_subset);
  sceneManager->update_subset_location(subset);

  //Make cleaning on the current subset
  if(with_filter_sphere){
    filterManager->filter_sphere_subset(subset);
  }

  //If camera follow up option activated
  followManager->camera_followup(cloud, ID_subset);

  //Colorization
  colorManager->make_colorization(cloud, ID_subset);

  //Update dynamic interfaces
  this->compute_recording(cloud, ID_subset);

  //---------------------------
  this->time_ope = stop_chrono(t1);
  this->compute_displayStats(subset);
}

//Subfunctions
void Online::compute_recording(Cloud* cloud, int& ID_subset){
  Node_interface* node_interface = node_engine->get_node_interface();
  Recorder* recordManager = node_interface->get_recordManager();
  //---------------------------

  recordManager->compute_online(cloud, ID_subset);

  //---------------------------
}
void Online::compute_displayStats(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Consol result
  string stats = subset->name + ": ope in ";
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
