#ifndef PLAYER_H
#define PLAYER_H

#include "../../common.h"

class Module_node;
class Obstacle;
class Online;
class Offline;
class Scenario;


class Player
{
public:
  //Constructor / Destructor
  Player(Module_node* node_module);
  ~Player();

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
