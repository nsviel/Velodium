#include "Module_node.h"

#include "SLAM/Module_slam.h"
#include "Radiometry/Module_radiometry.h"
#include "Registration/Module_registration.h"
#include "Obstacle/Module_obstacle.h"

#include "../Engine/Engine_node.h"
#include "../Engine/Scene/Configuration.h"
#include "../Operation/Node_operation.h"


//Constructor / destructor
Module_node::Module_node(Engine_node* node){
  //---------------------------

  this->node_engine = node;
  this->node_ope = node->get_node_ope();

  this->module_radio = new Module_radiometry(this);
  this->module_regist = new Module_registration(this);
  this->module_slam = new Module_slam(this);
  this->module_obstacle = new Module_obstacle(this);

  //---------------------------
}
Module_node::~Module_node(){}

void Module_node::reset(){
  //---------------------------

  module_slam->reset();

  //---------------------------
}
void Module_node::update(){
  //---------------------------

  module_slam->update();

  //---------------------------
}
void Module_node::runtime(){
  //---------------------------


  //---------------------------
}
