#ifndef INTERFACE_NODE_H
#define INTERFACE_NODE_H

#include "../../common.h"

class GPS;
class Saving;
class Prediction;
class Module_node;
class Network;
class Capture;
class Watcher;
class Filemanager;
class Engine_node;


class Interface_node
{
public:
  //Constructor / Destructor
  Interface_node(Module_node* node_module);
  ~Interface_node();

public:
  //Main functions
  void init();
  void runtime();

  inline GPS* get_gpsManager(){return gpsManager;}
  inline Saving* get_saveManager(){return saveManager;}
  inline Prediction* get_predManager(){return predManager;}
  inline Network* get_netManager(){return netManager;}
  inline Capture* get_captureManager(){return captureManager;}
  inline Engine_node* get_node_engine(){return node_engine;}
  inline Filemanager* get_fileManager(){return fileManager;}
  
private:
  Engine_node* node_engine;
  GPS* gpsManager;
  Saving* saveManager;
  Prediction* predManager;
  Network* netManager;
  Capture* captureManager;
  Filemanager* fileManager;
  Watcher* watcherManager;
};

#endif
