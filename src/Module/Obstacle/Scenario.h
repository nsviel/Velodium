#ifndef SCENARIO_H
#define SCENARIO_H

#include "../../common.h"

class Module_node;
class Interfacing;
class Capture;
class Network;
class SLAM_configuration;
class Player_online;


class Scenario
{
public:
  //Constructor / Destructor
  Scenario(Module_node* node_module);
  ~Scenario();

public:
  void scenario_start();
  void scenario_stop();

  void scenario_WP4_auto();
  void scenario_WP5_train_board();
  void scenario_WP5_train_edge();

  inline int* get_scenario_selected(){return &scenario_selected;}
  inline bool* get_scenario_started(){return &scenario_started;}

private:
  Interfacing* ioManager;
  Capture* captureManager;
  Network* netManager;
  SLAM_configuration* slam_configManager;
  Player_online* onlineManager;

  int scenario_selected;
  bool scenario_started;
};

#endif
