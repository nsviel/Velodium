#include "Online.h"
#include "Saving.h"

#include "../Node_operation.h"

#include "../../Module/Node_module.h"

#include "../../Interface/Network/HTTP/http_command.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Color/Color.h"
#include "../../Operation/Transformation/Transforms.h"
#include "../../Operation/Transformation/Filter.h"

#include "../../Engine/OpenGL/Camera/Renderer.h"
#include "../../Engine/OpenGL/Camera/Followup.h"
#include "../../Engine/OpenGL/Dimension.h"
#include "../../Engine/Engine.h"
#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Object.h"
#include "../../Engine/Scene/Configuration.h"

#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_system.h"
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
  this->savingManager = node_ope->get_savingManager();
  this->httpManager = new http_command();

  //---------------------------
  this->update_configuration();
}
Online::~Online(){}

//Main function
void Online::update_configuration(){
  //---------------------------

  this->time_operation = 0;
  this->visibility_range = 15;
  this->with_subset_specific_color = false;
  this->with_cylinder_cleaning = configManager->parse_json_b("module", "with_cylinder_cleaning");

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

  //Make slam on the current subset
  node_module->online(cloud, ID_subset);

  //Make cleaning on the current subset
  if(with_cylinder_cleaning){
    filterManager->filter_subset_cylinder(subset);
  }

  //If camera follow up option activated
  followManager->camera_followUp(cloud, ID_subset);

  //Colorization
  colorManager->make_colorization(cloud, ID_subset);

  //Control subset visibilities
  this->compute_visibility(cloud, ID_subset);

  //Update dynamic interfaces
  savingManager->compute_online(cloud, ID_subset);

  //Update dynamic glyphs
  objectManager->update_dynamic(cloud);

  //---------------------------
  this->time_operation = stop_chrono(t1);
  this->compute_displayStats(subset);
}

//Subfunctions
void Online::compute_visibility(Cloud* cloud, int& ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  if(subset == nullptr) return;
  //---------------------------

  //Set visibility just for wanted subsets
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->ID > ID_subset - visibility_range - 1 && subset->ID <= ID_subset){
      subset->visibility = true;
    }else{
      subset->visibility = false;
    }
  }

  //---------------------------
}
void Online::compute_displayStats(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  //Consol result
  string stats = subset->name + ": ope in ";
  stats += to_string((int)time_operation) + " ms";
  console.AddLog("#", stats);

  //---------------------------
}
void Online::compute_http_command(){
  //---------------------------

  vector<vector<string>> option = httpManager->parse_http_config();

  for(int i=0; i<option.size(); i++){
    string opt = option[i][0];
    string val = option[i][1];

    if(opt == "slam"){
      this->with_slam = string_to_bool(val);
    }
    else if(opt == "view"){
      followManager->camera_mode(val);
    }
  }

  //---------------------------
}

//Visibility function
void Online::set_visibility_range(int value){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  this->visibility_range = value;

  //---------------------------
}
int Online::get_visibility_range_max(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  int range_max = 15;
  //---------------------------

  if(cloud != nullptr && sceneManager->get_is_list_empty() == false){
    if(cloud->nb_subset > 15){
      range_max = cloud->nb_subset;
    }
  }

  //---------------------------
  return range_max;
}
