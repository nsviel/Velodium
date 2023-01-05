#ifndef GUI_INITIALIZATION_H
#define GUI_INITIALIZATION_H

#include "../../common.h"

class Node_gui;
class Scene;
class Loader;
class Pather;
class Configuration;
class Capture;

struct tree_file{
  string          name;
  string          type;
  string          path;
  float           size = -1;
  bool            already_open = false;
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
  void construst_tree();
  void construct_node(string path, vector<tree_file*>& nodes);
  void construct_node_root(vector<string>& vec_path, vector<tree_file*>& nodes);
  void display_node(tree_file* node, vector<tree_file*>& all_nodes);
  void display_node_root(vector<tree_file*>& all_nodes);
  void node_child_scan(string path, vector<tree_file*>& nodes, tree_file* parent);
  bool check_file_format(string path);
  void open_selection(tree_file* node);

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Pather* pathManager;
  Configuration* configManager;
  Capture* captureManager;

  vector<tree_file*> nodes_root;
  vector<tree_file*> nodes_path_1;
  vector<tree_file*> nodes_path_2;
  vector<tree_file*> nodes_path_3;
  vector<string> accepted_format;
  bool with_remove_cloud;
  bool with_onthefly;
  int cloud_scale;
  string lidar_model;
  string path_1;
  string path_2;
  string path_3;
};

#endif
