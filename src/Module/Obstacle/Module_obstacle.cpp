#include "Module_obstacle.h"

#include "src/Obstacle.h"
#include "src/Filemanager.h"
#include "src/Prediction.h"
#include "src/Warning.h"
#include "HTTP/HTTP.h"
#include "MQTT/MQTT.h"
#include "GUI/GUI_Obstacle.h"
#include "GUI/GUI_Network.h"

#include "../Module_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Operation/Node_operation.h"


//Constructor / Destructor
Module_obstacle::Module_obstacle(Module_node* node){
  this->node_module = node;
  //---------------------------

  this->node_engine = node->get_node_engine();
  this->node_ope = node->get_node_ope();
  this->configManager = node_engine->get_configManager();
  this->obstacleManager = new Obstacle(this);
  this->predManager = new Prediction(this);
  this->fileManager = new Filemanager(this);
  this->gui_obstacle = new GUI_Obstacle(this);
  this->warningManager = new Warning(this);
  this->mqttManager = new MQTT(this);
  this->httpsManager = new HTTP(this);

  //---------------------------
}
Module_obstacle::~Module_obstacle(){}

void Module_obstacle::init(){
  //---------------------------

  fileManager->check_directories();
  fileManager->clean_directories();

  //---------------------------
}
void Module_obstacle::update(){
  //---------------------------

  fileManager->update_configuration();
  httpsManager->update_configuration();
  mqttManager->update_configuration();

  //---------------------------
}
void Module_obstacle::runtime(){
  //---------------------------

  obstacleManager->runtime();
  //gui_obstacle->runtime_display_naming();

  //---------------------------
}
void Module_obstacle::reset(){
  //---------------------------


  //---------------------------
}
