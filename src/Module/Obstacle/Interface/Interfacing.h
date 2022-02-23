#ifndef INTERFACING_H
#define INTERFACING_H

#include "../../../common.h"

class GPS;
class Saving;
class Prediction;
class Module_node;


class Interfacing
{
public:
  //Constructor / Destructor
  Interfacing(Module_node* node_module);
  ~Interfacing();

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
  inline string get_dir_path(){return path_dir;}

private:
  GPS* gpsManager;
  Saving* saveManager;
  Prediction* predManager;

  bool is_clean_dir;
  string path_dir;
};

#endif
