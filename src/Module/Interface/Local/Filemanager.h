#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../../../common.h"

class Interface_node;
class Configuration;


class Filemanager
{
public:
  //Constructor / Destructor
  Filemanager(Interface_node* node_interface);
  ~Filemanager();

public:
  void update_configuration();
  void clean_directories();
  void check_directories();
  void select_dir_path();

  inline string get_path_data_dir(){return path_data_dir;}

private:
  Interface_node* node_interface;
  Configuration* configManager;

  string path_data_dir;
  bool is_clean_dir;
};

#endif
