#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../../../common.h"

class Module_obstacle;
class Configuration;
class Prediction;


class Filemanager
{
public:
  //Constructor / Destructor
  Filemanager(Module_obstacle* module);
  ~Filemanager();

public:
  void update_configuration();
  void clean_directories();
  void check_directories();
  void select_dir_path();

  inline string get_path_data_dir(){return path_data_dir;}

private:
  Configuration* configManager;
  Prediction* predManager;

  string path_data_dir;
  bool is_clean_dir;
};

#endif
