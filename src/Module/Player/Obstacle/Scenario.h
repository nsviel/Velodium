#ifndef SCENARIO_H
#define SCENARIO_H

#include "../../../common.h"

class Module_node;
class Capture;
class Network;
class SLAM_configuration;
class Online;
class Prediction;
class Configuration;
class GPS;


class Scenario
{
public:
  //Constructor / Destructor
  Scenario(Module_node* node_module, Online* online);
  ~Scenario();

public:
  void scenario_start();
  void scenario_stop();

  //Subfunctions
  void make_configuration();
  void make_watcher();

  inline int* get_scenario_selected(){return &scenario_selected;}
  inline bool* get_scenario_started(){return &scenario_started;}

private:
  Capture* captureManager;
  Network* netManager;
  Configuration* configManager;
  SLAM_configuration* slam_configManager;
  Online* onlineManager;
  Prediction* predManager;
  GPS* gpsManager;

  int scenario_selected;
  bool scenario_started;
};

#endif
