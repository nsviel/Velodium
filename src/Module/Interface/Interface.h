#ifndef INTERFACE_H
#define INTERFACE_H

#include "../../common.h"

class GPS;
class Saving;
class Prediction;
class Module_node;
class Network;
class Capture;


class Interface
{
public:
  //Constructor / Destructor
  Interface(Module_node* node_module);
  ~Interface();

public:
  //Main functions
  void init();
  void runtime();

  //Subfunctions
  void clean_directories();
  void check_directories();
  void select_dir_path();

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

  bool is_clean_dir;
  string path_dir;
};

#endif