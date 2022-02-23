#include "Player.h"

#include "Dynamic/Online.h"
#include "Dynamic/Offline.h"
#include "Obstacle/Obstacle.h"
#include "Obstacle/Scenario.h"

#include "../Module_node.h"


//Constructor / Destructor
Player::Player(Module_node* node_module){
  //---------------------------

  this->onlineManager = new Online(node_module);
  this->offlineManager = new Offline(onlineManager);
  this->obstacleManager = new Obstacle(node_module);
  this->scenarioManager = new Scenario(node_module, onlineManager);

  //---------------------------
}
Player::~Player(){}

void Player::update(){
  //---------------------------

  onlineManager->update_configuration();

  //---------------------------
}
void Player::runtime(){
  //---------------------------

  obstacleManager->runtime_obstacle();

  //---------------------------
}
