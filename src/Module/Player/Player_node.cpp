#include "Player_node.h"

#include "Dynamic/Online.h"
#include "Dynamic/Offline.h"
#include "Dynamic/Followup.h"
#include "Obstacle/Obstacle.h"

#include "../Module_node.h"


//Constructor / Destructor
Player_node::Player_node(Module_node* node){
  this->node_module = node;
  //---------------------------

  this->onlineManager = new Online(node_module);
  this->offlineManager = new Offline(node_module, onlineManager);
  this->obstacleManager = new Obstacle(node_module);
  this->followManager = new Followup(this);

  //---------------------------
}
Player_node::~Player_node(){}

void Player_node::update(){
  //---------------------------

  onlineManager->update_configuration();

  //---------------------------
}
void Player_node::runtime(){
  //---------------------------

  obstacleManager->runtime_obstacle();
  offlineManager->player_runtime();

  //---------------------------
}
void Player_node::reset(){
  //---------------------------

  followManager->camera_reset();

  //---------------------------
}
