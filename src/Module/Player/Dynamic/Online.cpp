#include "Online.h"

#include "Followup.h"

#include "../../Module_node.h"
#include "../../SLAM/Slam.h"
#include "../../Interface/Interface_node.h"
#include "../../Interface/Local/Saving.h"
#include "../../Interface/LiDAR/Capture.h"

#include "../../../Operation/Operation_node.h"
#include "../../../Operation/Color/Color.h"
#include "../../../Operation/Transformation/Transforms.h"
#include "../../../Operation/Transformation/Filter.h"

#include "../../../Engine/OpenGL/Camera/Renderer.h"
#include "../../../Engine/OpenGL/Dimension.h"

#include "../../../Engine/Engine.h"
#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Configuration.h"

#include "../../../Specific/fct_maths.h"
#include "../../../Specific/fct_transtypage.h"
#include "../../../Specific/fct_system.h"


//Constructor / Destructor
Online::Online(Module_node* node_module){
  //---------------------------

  Operation_node* node_ope = node_module->get_node_ope();
  Engine_node* node_engine = node_module->get_node_engine();
  Interface_node* node_interface = node_module->get_node_interface();

  this->filterManager = node_ope->get_filterManager();
  this->dimManager = node_engine->get_dimManager();
  this->slamManager = node_module->get_slamManager();
  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->colorManager = node_ope->get_colorManager();
  this->saveManager = node_interface->get_saveManager();
  this->captureManager = node_interface->get_captureManager();

  this->visibility_range = 15;

  //---------------------------
  this->update_configuration();
}
Online::~Online(){}

//Main function
void Online::update_configuration(){
  //---------------------------

  this->nb_subset_max = 50;
  this->with_justOneFrame = false;
  this->with_subset_specific_color = false;
  this->with_save_frame = configManager->parse_json_b("interface", "with_save_frame");
  this->with_save_image = configManager->parse_json_b("interface", "with_save_image");
  this->with_slam = configManager->parse_json_b("module", "with_slam");
  this->with_cylinder_cleaning = configManager->parse_json_b("module", "with_cylinder_cleaning");
  this->with_remove_lastSubset = configManager->parse_json_b("interface", "with_remove_lastSubset");

  //---------------------------
}
void Online::compute_onlineOpe(Cloud* cloud, int ID_subset){
  //This function is called each time a new subset arrives
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  if(subset == nullptr) return;
  cloud->subset_selected = subset;
  //---------------------------

  //Control subset visibilities
  this->compute_visibility(cloud, ID_subset);

  //Make slam on the current subset
  if(with_slam){
    slamManager->compute_slam_online(cloud, ID_subset);
  }

  //If camera follow up option activated
  /*if(with_camera_follow){
    this->camera_followUp(cloud, ID_subset);
  }
  //Ortho top view option
  if(with_camera_top){
    cameraManager->input_projView(2);
  }*/

  //Make cleaning on the current subset
  if(with_cylinder_cleaning){
    filterManager->filter_subset_cylinder(subset);
  }

  //Save subset frame
  if(with_save_frame){
    saveManager->save_frame(subset);
  }

  //Save rendered image
  if(with_save_image){
    saveManager->save_image();
  }

  //Colorization
  this->compute_colorization(cloud, ID_subset);

  //Regulate the number of cloud frame
  this->compute_size_controler(cloud);

  //Provide info about computation
  this->compute_displayStats(subset);

  //---------------------------
}

//Subfunctions
void Online::compute_colorization(Cloud* cloud, int ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  //---------------------------

  if(with_subset_specific_color){
    colorManager->make_colorization_specific(subset);
    if(ID_subset>2){
      Subset* subset_m1 = sceneManager->get_subset_byID(cloud, ID_subset-1);
      colorManager->make_colorization(subset_m1);
    }
  }else{
    colorManager->make_colorization(subset);
  }

  //---------------------------
}
void Online::compute_size_controler(Cloud* cloud){
  //---------------------------

  //If option, remove all other subset
  if(with_justOneFrame){
    sceneManager->remove_subset_last(cloud);
  }
  //Remove old frame if option is activated
  else{
    bool is_capturing = captureManager->get_is_capture_watcher();
    if(cloud->subset.size() > nb_subset_max && is_capturing){
      sceneManager->remove_subset_last(cloud);
    }
  }

  //---------------------------
}
void Online::compute_visibility(Cloud* cloud, int& ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  if(subset == nullptr) return;
  //---------------------------

  //Set visibility just for wanted subsets
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);

    if(subset->ID > ID_subset - visibility_range && subset->ID <= ID_subset){
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
  string stats = subset->name + ": ";
  if(with_slam && frame->is_slamed){
    stats += "[SLAM- " + to_string((int)frame->time_slam) + " ms] ";
  }
  if(with_save_frame){
    stats += "[Frame- " + to_string((int)frame->time_save_frame) + " ms] ";
  }
  console.AddLog("#", stats);

  //---------------------------
}
