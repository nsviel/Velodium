#ifndef NODE_MODULE_H
#define NODE_MODULE_H

#include "SLAM/Slam.h"
#include "LiDAR/Capture.h"
#include "Obstacle/Obstacle.h"
#include "Obstacle/Interface/Interfacing.h"
#include "Obstacle/Scenario.h"
#include "Player/Player_cloud.h"
#include "Player/Player_online.h"
#include "Network/Network.h"

#include "../Engine/Scene/Configuration.h"
#include "../Engine/Engine_node.h"
#include "../Operation/Operation_node.h"
#include "../common.h"


class Module_node
{
public:
  //Constructor / Destructor
  Module_node(Engine_node* engine, Operation_node* ope){
    //---------------------------

    this->node_engine = engine;
    this->node_ope = ope;

    //---------------------------
    this->init_objects();
  }
  ~Module_node(){}

public:
  //Main functions
  void init_objects(){
    //---------------------------

    this->slamManager = new Slam();
    this->netManager = new Network();
    this->ioManager = new Interfacing(this);
    this->obstacleManager = new Obstacle(this);
    this->onlineManager = new Player_online(this);
    this->playerManager = new Player_cloud(onlineManager);
    this->captureManager = new Capture(this);
    this->scenarioManager = new Scenario(this);

    //---------------------------
  }
  void reset(){
    //---------------------------

    slamManager->reset_slam();

    //---------------------------
  }
  void update(){
    //---------------------------

    onlineManager->update_configuration();

    //---------------------------
  }
  void runtime(){
    //---------------------------

    captureManager->runtime_capturing();
    obstacleManager->runtime_obstacle();

    //---------------------------
  }

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Operation_node* get_node_ope(){return node_ope;}

  inline Network* get_netManager(){return netManager;}
  inline Slam* get_slamManager(){return slamManager;}
  inline Capture* get_captureManager(){return captureManager;}
  inline Obstacle* get_obstacleManager(){return obstacleManager;}
  inline Interfacing* get_ioManager(){return ioManager;}
  inline Player_online* get_onlineManager(){return onlineManager;}
  inline Player_cloud* get_playerManager(){return playerManager;}
  inline Scenario* get_scenarioManager(){return scenarioManager;}

private:
  Engine_node* node_engine;
  Operation_node* node_ope;

  Network* netManager;
  Slam* slamManager;
  Capture* captureManager;
  Configuration* configManager;
  Obstacle* obstacleManager;
  Interfacing* ioManager;
  Player_cloud* playerManager;
  Player_online* onlineManager;
  Scenario* scenarioManager;
};

#endif
