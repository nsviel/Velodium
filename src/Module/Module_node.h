#ifndef NODE_MODULE_H
#define NODE_MODULE_H

#include "SLAM/CT_ICP.h"
#include "Player/Player_cloud.h"
#include "Player/Player_online.h"

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

    this->node_engineManager = engine;
    this->node_opeManager = ope;
    this->init_objects();

    //---------------------------
  }
  ~Module_node(){}

public:
  //Main functions
  void init_objects(){
    //---------------------------

    this->cticpManager = new CT_ICP();
    //this->obstacleManager = new Obstacle();
    this->onlineManager = new Player_online(this);
    this->playerManager = new Player_cloud(onlineManager);

    //---------------------------
  }
  void reset(){
    //---------------------------
    cticpManager->reset();
    //---------------------------
  }
  void update(){}
  void runtime(){
    //---------------------------

    //---------------------------
  }

  inline CT_ICP* get_cticpManager(){return cticpManager;}
  inline Engine_node* get_node_engineManager(){return node_engineManager;}
  inline Operation_node* get_node_opeManager(){return node_opeManager;}
  inline Player_online* get_onlineManager(){return onlineManager;}
  inline Player_cloud* get_playerManager(){return playerManager;}

private:
  Engine_node* node_engineManager;
  Operation_node* node_opeManager;

  CT_ICP* cticpManager;
  config_module* configManager;
  Player_cloud* playerManager;
  Player_online* onlineManager;
};

#endif
