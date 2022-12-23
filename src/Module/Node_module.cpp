#include "Node_module.h"

#if defined(WITH_SLAM)
#include "SLAM/Module_slam.h"
#endif
#if defined(WITH_RADIOMETRY)
#include "Radiometry/Module_radiometry.h"
#endif
#if defined(WITH_REGISTRATION)
#include "Registration/Module_registration.h"
#endif
#if defined(WITH_OBSTACLE)
#include "Obstacle/Module_obstacle.h"
#endif

#include "../Engine/Node_engine.h"
#include "../Operation/Node_operation.h"
#include "../Interface/Node_interface.h"


//Constructor / destructor
Node_module::Node_module(Node_engine* node){
  //---------------------------

  this->node_engine = node;
  this->node_ope = node->get_node_ope();
  this->node_interface = node->get_node_interface();

  //---------------------------
  this->load_module();
}
Node_module::~Node_module(){}

void Node_module::load_module(){
  //---------------------------

  if(WITH_RADIOMETRY){
    this->module_radio = new Module_radiometry(this);
  }else{
    this->module_radio = nullptr;
  }

  if(WITH_REGISTRATION){
    this->module_regist = new Module_registration(this);
  }else{
    this->module_regist = nullptr;
  }

  if(WITH_SLAM){
    this->module_slam = new Module_slam(this);
  }else{
    this->module_slam = nullptr;
  }

  if(WITH_OBSTACLE){
    this->module_obstacle = new Module_obstacle(this);
  }else{
    this->module_obstacle = nullptr;
  }

  //---------------------------
}
void Node_module::reset(){
  //---------------------------

  if(WITH_SLAM){
    module_slam->reset();
  }

  //---------------------------
}
void Node_module::update(){
  //---------------------------

  if(WITH_OBSTACLE){
    module_obstacle->update();
  }

  if(WITH_SLAM){
    module_slam->update();
  }

  //---------------------------
}
void Node_module::runtime(){
  //---------------------------

  if(WITH_OBSTACLE){
    module_obstacle->runtime();
  }

  //---------------------------
}
void Node_module::draw(){
  if(ImGui::BeginTabItem("Module")){
    if(ImGui::BeginTabBar("##tabs_module", ImGuiTabBarFlags_None)){
      //---------------------------

      if(WITH_SLAM){
        module_slam->draw();
      }

      if(WITH_OBSTACLE){
        module_obstacle->draw();
      }

      //---------------------------
      ImGui::EndTabBar();
    }
    ImGui::EndTabItem();
  }
}
void Node_module::draw_online(){
  //---------------------------

  if(WITH_SLAM){
    module_slam->draw_online();
  }

  //---------------------------
}
void Node_module::online(Cloud* cloud, int subset_ID){
  //---------------------------

  if(WITH_SLAM){
    module_slam->online(cloud, subset_ID);
  }

  //---------------------------
}
