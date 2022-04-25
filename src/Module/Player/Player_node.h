#ifndef PLAYER_NODE_H
#define PLAYER_NODE_H

#include "../../common.h"

class Module_node;
class Obstacle;
class Online;
class Offline;
class Followup;


class Player_node
{
public:
  //Constructor / Destructor
  Player_node(Module_node* node_module);
  ~Player_node();

public:
  void update();
  void runtime();
  void reset();

  inline Module_node* get_node_module(){return node_module;}
  inline Online* get_onlineManager(){return onlineManager;}
  inline Offline* get_offlineManager(){return offlineManager;}
  inline Obstacle* get_obstacleManager(){return obstacleManager;}
  inline Followup* get_followManager(){return followManager;}

private:
  Module_node* node_module;
  Obstacle* obstacleManager;
  Online* onlineManager;
  Offline* offlineManager;
  Followup* followManager;
};

#endif
