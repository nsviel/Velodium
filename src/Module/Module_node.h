#ifndef NODE_MODULE_H
#define NODE_MODULE_H

#include "SLAM/CT_ICP.h"
#include "LiDAR/Capture.h"
#include "Obstacle/Obstacle.h"
#include "Obstacle/Interfacing.h"
#include "Player/Player_cloud.h"
#include "Player/Player_online.h"
#include "Network/Network.h"

#include "../Engine/Configuration/config_module.h"
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

    this->cticpManager = new CT_ICP();
    this->netManager = new Network();
    this->ioManager = new Interfacing(this);
    this->obstacleManager = new Obstacle(this);
    this->onlineManager = new Player_online(this);
    this->playerManager = new Player_cloud(onlineManager);
    this->captureManager = new Capture(this);

    //---------------------------
  }
  void reset(){
    //---------------------------

    cticpManager->reset_slam();

    //---------------------------
  }
  void update(){}
  void runtime(){
    //---------------------------

    captureManager->runtime_capturing();
    obstacleManager->runtime_obstacle();

    //---------------------------
  }

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Operation_node* get_node_ope(){return node_ope;}

  inline Network* get_netManager(){return netManager;}
  inline CT_ICP* get_cticpManager(){return cticpManager;}
  inline Capture* get_captureManager(){return captureManager;}
  inline Obstacle* get_obstacleManager(){return obstacleManager;}
  inline Interfacing* get_ioManager(){return ioManager;}
  inline Player_online* get_onlineManager(){return onlineManager;}
  inline Player_cloud* get_playerManager(){return playerManager;}

private:
  Engine_node* node_engine;
  Operation_node* node_ope;

  Network* netManager;
  CT_ICP* cticpManager;
  Capture* captureManager;
  config_module* configManager;
  Obstacle* obstacleManager;
  Interfacing* ioManager;
  Player_cloud* playerManager;
  Player_online* onlineManager;
};

#endif
