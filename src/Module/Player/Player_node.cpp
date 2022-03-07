#include "Player_node.h"

#include "Dynamic/Online.h"
#include "Dynamic/Offline.h"
#include "Obstacle/Obstacle.h"
#include "Obstacle/Scenario.h"

#include "../Module_node.h"


//Constructor / Destructor
Player_node::Player_node(Module_node* node_module){
  //---------------------------

  this->onlineManager = new Online(node_module);
  this->offlineManager = new Offline(node_module, onlineManager);
  this->obstacleManager = new Obstacle(node_module);
  this->scenarioManager = new Scenario(node_module, onlineManager);

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
