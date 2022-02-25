#ifndef PLAYER_NODE_H
#define PLAYER_NODE_H

#include "../../common.h"

class Module_node;
class Obstacle;
class Online;
class Offline;
class Scenario;


class Player_node
{
public:
  //Constructor / Destructor
  Player_node(Module_node* node_module);
  ~Player_node();

public:
  void update();
  void runtime();

  inline Online* get_onlineManager(){return onlineManager;}
  inline Offline* get_offlineManager(){return offlineManager;}
  inline Obstacle* get_obstacleManager(){return obstacleManager;}
  inline Scenario* get_scenarioManager(){return scenarioManager;}

private:
  Obstacle* obstacleManager;
  Online* onlineManager;
  Offline* offlineManager;
  Scenario* scenarioManager;
};

#endif
