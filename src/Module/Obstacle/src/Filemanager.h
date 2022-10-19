#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../../../common.h"

class Module_obstacle;
class Configuration;


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
  inline string get_path_grThr(){return path_grThr;}
  inline string get_path_predi(){return path_predi;}

private:
  Configuration* configManager;

  string path_data_dir;
  string path_predi;
  string path_grThr;
  bool is_clean_dir;
};

#endif
