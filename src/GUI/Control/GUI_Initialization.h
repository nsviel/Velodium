#ifndef GUI_INITIALIZATION_H
#define GUI_INITIALIZATION_H

#include "../../common.h"

class Node_gui;
class Scene;
class Loader;
class Pather;
class Configuration;

struct tree_file{
  string          name;
  string          type;
  string          path;
  float           size;
  bool            end_folder;
  int             leaf_idx;
  int             leaf_nb;
};


class GUI_Initialization
{
public:
  //Constructor / Destructor
  GUI_Initialization(Node_gui* node_gui);
  ~GUI_Initialization();

public:
  void init_gui();
  void update_configuration();

  void operation_cloud(Cloud* cloud);
  void operation_option();

  void treeview();
  void display_node(tree_file* node, vector<tree_file*>& all_nodes);
  void recursive_scan(string path, vector<tree_file*>& nodes, tree_file* parent);
  bool check_file_format(string path);
  void construct_tree(string path, vector<tree_file*>& nodes);
  void open_selection(tree_file* node);

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Pather* pathManager;
  Configuration* configManager;

  vector<string> accepted_format;
  bool remove_cloud;
  int cloud_scale;
  string lidar_model;
  string path_1;
  string path_2;
  string path_3;
};

#endif
