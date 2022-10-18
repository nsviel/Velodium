#include "Module_registration.h"

#include "../Module_node.h"


//Constructor / Destructor
Module_registration::Module_registration(Module_node* node){
  this->node_module = node;
  //---------------------------

  this->node_engine = node->get_node_engine();
  this->node_ope = node_engine->get_node_ope();

  //---------------------------
}
Module_registration::~Module_registration(){}

void Module_registration::init(){
  //---------------------------

  //---------------------------
}
void Module_registration::update(){
  //---------------------------

  //---------------------------
}
void Module_registration::runtime(){
  //---------------------------


  //---------------------------
}
void Module_registration::reset(){
  //---------------------------


  //---------------------------
}
