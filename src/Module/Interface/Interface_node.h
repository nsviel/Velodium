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

  inline string get_dir_path(){return path_dir;}

private:
  GPS* gpsManager;
  Saving* saveManager;
  Prediction* predManager;
  Network* netManager;
  Capture* captureManager;
  Filemanager* fileManager;
  Watcher* watcherManager;

  string path_dir;
};

#endif
