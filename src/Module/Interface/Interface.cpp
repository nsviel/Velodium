#include "Interface.h"

#include "Interface_node.h"
#include "Local/Saving.h"
#include "Local/GPS.h"
#include "Local/Prediction.h"
#include "LiDAR/Capture.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Engine/Scene/Scene.h"


//Constructor / Destructor
Interface::Interface(Interface_node* node){
  //---------------------------

  Engine_node* node_engine = node->get_node_engine();
  Configuration* configManager = node_engine->get_configManager();

  this->sceneManager = node_engine->get_sceneManager();
  this->captureManager = node->get_captureManager();
  this->gpsManager = node->get_gpsManager();
  this->predManager = node->get_predManager();
  this->saveManager = node->get_saveManager();
  this->captureManager = node->get_captureManager();

  this->nb_subset_max = 50;
  this->with_justOneFrame = false;
  this->with_save_frame = configManager->parse_json_b("interface", "with_save_frame");
  this->with_save_image = configManager->parse_json_b("interface", "with_save_image");

  //---------------------------
}
Interface::~Interface(){}

void Interface::runtime_loop(){
  //---------------------------

  captureManager->runtime_capturing();
  gpsManager->runtime_gps();
  predManager->runtime_prediction();

  //---------------------------
}
void Interface::dynamic_loop(Cloud* cloud, int ID_subset){
  Subset* subset = sceneManager->get_subset_byID(cloud, ID_subset);
  //---------------------------

  //Save subset frame
  if(with_save_frame){
    saveManager->save_frame(subset);
  }

  //Save rendered image
  if(with_save_image){
    saveManager->save_image();
  }

  //Regulate the number of scene subsets
  this->controler_nb_subset(cloud);

  //---------------------------
}
void Interface::controler_nb_subset(Cloud* cloud){
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
