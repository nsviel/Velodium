#include "Module_radiometry.h"

#include "../Node_module.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Operation/Node_operation.h"


//Constructor / Destructor
Module_radiometry::Module_radiometry(Node_module* node){
  this->node_module = node;
  //---------------------------

  this->node_engine = node->get_node_engine();
  this->node_ope = node_engine->get_node_ope();
  this->node_load = node_engine->get_node_load();

  //---------------------------
}
Module_radiometry::~Module_radiometry(){}

void Module_radiometry::init(){
  //---------------------------


  //---------------------------
}
void Module_radiometry::update(){
  //---------------------------


  //---------------------------
}
void Module_radiometry::runtime(){
  //---------------------------


  //---------------------------
}
void Module_radiometry::reset(){
  //---------------------------


  //---------------------------
}
