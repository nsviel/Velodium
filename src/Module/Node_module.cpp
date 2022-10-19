#include "Node_module.h"

#include "SLAM/Module_slam.h"
#include "Radiometry/Module_radiometry.h"
#include "Registration/Module_registration.h"
#include "Obstacle/Module_obstacle.h"

#include "../Engine/Node_engine.h"
#include "../Engine/Scene/Configuration.h"
#include "../Operation/Node_operation.h"


//Constructor / destructor
Node_module::Node_module(Node_engine* node){
  //---------------------------

  this->node_engine = node;
  this->node_ope = node->get_node_ope();

  this->module_radio = new Module_radiometry(this);
  this->module_regist = new Module_registration(this);
  this->module_slam = new Module_slam(this);
  this->module_obstacle = new Module_obstacle(this);

  //---------------------------
}
Node_module::~Node_module(){}

void Node_module::reset(){
  //---------------------------

  module_slam->reset();

  //---------------------------
}
void Node_module::update(){
  //---------------------------

  module_obstacle->update();
  module_slam->update();

  //---------------------------
}
void Node_module::runtime(){
  //---------------------------

  module_obstacle->runtime();

  //---------------------------
}
void Node_module::draw(){
  //---------------------------

  module_obstacle->draw();
  module_slam->draw();

  //---------------------------
}
