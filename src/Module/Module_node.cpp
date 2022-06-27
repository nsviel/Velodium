#include "Module_node.h"

#include "SLAM/Slam.h"
#include "Interface/Interface_node.h"
#include "Player/Player_node.h"

#include "../Engine/Engine_node.h"
#include "../Engine/Scene/Configuration.h"
#include "../Operation/Operation_node.h"


//Constructor / destructor
Module_node::Module_node(Engine_node* engine){
  this->node_engine = engine;
  //---------------------------

  this->node_ope = node_engine->get_node_ope();

  this->slamManager = new Slam(node_engine);
  this->node_interface = new Interface_node(this);
  this->node_player = new Player_node(this);

  //---------------------------
}
Module_node::~Module_node(){}

void Module_node::reset(){
  //---------------------------

  slamManager->reset_slam_hard();
  node_player->reset();

  //---------------------------
}
void Module_node::update(){
  //---------------------------

  node_player->update();
  node_interface->update();
  slamManager->update_configuration();

  //---------------------------
}
void Module_node::runtime(){
  //---------------------------

  node_interface->runtime();
  node_player->runtime();

  //---------------------------
}
