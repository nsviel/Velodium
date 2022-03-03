#ifndef WATCHER_H
#define WATCHER_H

#include "../../../common.h"

class Engine_node;
class Interface_node;
class Capture;
class Prediction;
class GPS;
class Configuration;


class Watcher
{
public:
  //Constructor / Destructor
  Watcher(Interface_node* node_interface, Engine_node* node_engine);
  ~Watcher();

public:
  void watcher_starter();

private:
  GPS* gpsManager;
  Capture* captureManager;
  Prediction* predManager;
  Configuration* configManager;
};

#endif
