#include "Module_obstacle.h"

#include "src/Obstacle.h"
#include "src/Filemanager.h"
#include "src/Prediction.h"
#include "src/Warning.h"
#include "GUI/GUI_Obstacle.h"

#include "../Node_module.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Operation/Node_operation.h"
#include "../../Interface/Node_interface.h"


//Constructor / Destructor
Module_obstacle::Module_obstacle(Node_module* node){
  this->node_module = node;
  //---------------------------

  this->node_engine = node->get_node_engine();
  this->node_ope = node->get_node_ope();
  this->node_interface = node->get_node_interface();

  this->configManager = node_engine->get_configManager();
  this->warningManager = new Warning(this);
  this->fileManager = new Filemanager(this);
  this->predManager = new Prediction(this);
  this->obstacleManager = new Obstacle(this);

  this->gui_obstacle = new GUI_Obstacle(this);

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

  //---------------------------
}
void Module_obstacle::runtime(){
  //---------------------------

  obstacleManager->runtime();
  gui_obstacle->runtime_display_naming();

  //---------------------------
}
void Module_obstacle::reset(){
  //---------------------------


  //---------------------------
}
void Module_obstacle::draw(){
  //---------------------------

  gui_obstacle->design_obstacle();

  //---------------------------
}
void Module_obstacle::draw_online(){
  //---------------------------


  //---------------------------
}
