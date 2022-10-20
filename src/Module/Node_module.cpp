#include "Node_module.h"

#include "SLAM/Module_slam.h"
#include "Radiometry/Module_radiometry.h"
#include "Registration/Module_registration.h"
#include "Obstacle/Module_obstacle.h"

#include "../Engine/Node_engine.h"
#include "../Engine/Scene/Configuration.h"
#include "../Operation/Node_operation.h"
#include "../Interface/Node_interface.h"


//Constructor / destructor
Node_module::Node_module(Node_engine* node){
  //---------------------------

  this->node_engine = node;
  this->node_ope = node->get_node_ope();
  this->node_interface = node->get_node_interface();

  Configuration* configManager = node_engine->get_configManager();
  this->with_slam = configManager->parse_json_b("module", "with_slam");

  //---------------------------
  this->load_module();
}
Node_module::~Node_module(){}

void Node_module::load_module(){
  //---------------------------

  #if defined(WITH_RADIOMETRY)
    this->module_radio = new Module_radiometry(this);
  #endif

  #if defined(WITH_REGISTRATION)
    this->module_regist = new Module_registration(this);
  #endif

  #if defined(WITH_SLAM)
    this->module_slam = new Module_slam(this);
  #endif

  #if defined(WITH_OBSTACLE)
    this->module_obstacle = new Module_obstacle(this);
  #endif

  //---------------------------
}
void Node_module::reset(){
  //---------------------------

  #if defined(WITH_SLAM)
    module_slam->reset();
  #endif

  //---------------------------
}
void Node_module::update(){
  //---------------------------

  #if defined(WITH_OBSTACLE)
    module_obstacle->update();
  #endif

  #if defined(WITH_SLAM)
    module_slam->update();
  #endif

  //---------------------------
}
void Node_module::runtime(){
  //---------------------------

  #if defined(WITH_OBSTACLE)
    module_obstacle->runtime();
  #endif

  //---------------------------
}
void Node_module::draw(){
  //---------------------------

  #if defined(WITH_OBSTACLE)
    module_obstacle->draw();
  #endif

  #if defined(WITH_SLAM)
    module_slam->draw();
  #endif

  //---------------------------
}
void Node_module::online(Cloud* cloud, int subset_ID){
  //---------------------------

  #if defined(WITH_SLAM)
    if(with_slam){
      module_slam->online(cloud, subset_ID);
    }
  #endif

  //---------------------------
}
