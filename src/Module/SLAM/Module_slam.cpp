#include "Module_slam.h"

#include "src/SLAM.h"
#include "GUI/GUI_Slam.h"

#include "../Module_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Operation/Node_operation.h"


//Constructor / Destructor
Module_slam::Module_slam(Module_node* node){
  this->node_module = node;
  //---------------------------

  this->node_engine = node->get_node_engine();
  this->node_ope = node_engine->get_node_ope();

  this->slamManager = new SLAM(node_engine);
  this->gui_slam = new GUI_Slam(this);

  //---------------------------
}
Module_slam::~Module_slam(){}

void Module_slam::init(){
  //---------------------------


  //---------------------------
}
void Module_slam::update(){
  //---------------------------

  slamManager->update_configuration();

  //---------------------------
}
void Module_slam::runtime(){
  //---------------------------


  //---------------------------
}
void Module_slam::reset(){
  //---------------------------

  slamManager->reset_slam();

  //---------------------------
}
void Module_slam::draw(){
  //---------------------------

  gui_slam->design_SLAM();

  //---------------------------
}
