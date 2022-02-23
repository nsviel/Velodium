#ifndef NODE_MODULE_H
#define NODE_MODULE_H

#include "SLAM/Slam.h"
#include "Interface/Interface.h"
#include "Player/Player.h"

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
    this->ioManager = new Interface(this);
    this->playerManager = new Player(this);

    //---------------------------
  }
  void reset(){
    //---------------------------

    slamManager->reset_slam();

    //---------------------------
  }
  void update(){
    //---------------------------

    playerManager->update();

    //---------------------------
  }
  void runtime(){
    //---------------------------

    ioManager->runtime();
    playerManager->runtime();

    //---------------------------
  }

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Operation_node* get_node_ope(){return node_ope;}

  inline Slam* get_slamManager(){return slamManager;}
  inline Interface* get_ioManager(){return ioManager;}
  inline Player* get_playerManager(){return playerManager;}

private:
  Engine_node* node_engine;
  Operation_node* node_ope;

  Slam* slamManager;
  Interface* ioManager;
  Player* playerManager;
};

#endif
