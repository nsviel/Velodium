#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../../../common.h"

class GPS;
class Saving;
class Prediction;
class Interface_node;


class Filemanager
{
public:
  //Constructor / Destructor
  Filemanager(Interface_node* node_interface);
  ~Filemanager();

public:
  void clean_directories();
  void check_directories();
  void select_dir_path();

private:
  GPS* gpsManager;
  Saving* saveManager;
  Prediction* predManager;
  Interface_node* node_interface;
};

#endif
