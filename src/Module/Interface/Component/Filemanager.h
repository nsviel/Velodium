#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../../../common.h"

class GPS;
class Saving;
class Prediction;
class Interface;


class Filemanager
{
public:
  //Constructor / Destructor
  Filemanager(Interface* ioManager);
  ~Filemanager();

public:
  void clean_directories();
  void check_directories();
  void select_dir_path();

private:
  GPS* gpsManager;
  Saving* saveManager;
  Prediction* predManager;
};

#endif
