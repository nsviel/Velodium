#include "Module_slam.h"

#include "src/SLAM.h"

#include "../Module_node.h"


//Constructor / Destructor
Module_slam::Module_slam(Module_node* node){
  this->node_module = node;
  //---------------------------

  this->slamManager = new SLAM(this);
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
