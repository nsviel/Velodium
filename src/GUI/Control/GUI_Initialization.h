#ifndef GUI_INITIALIZATION_H
#define GUI_INITIALIZATION_H

#include "../../common.h"

class Node_gui;
class Scene;
class Loader;
class Pather;
class Configuration;
class Capture;
class Transformation;

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
  //Main funxtion
  void init_gui();
  void update_configuration();

  //Operation on loaded cloud
  void operation_new_collection(Collection* collection);
  void operation_option();

  //Spacific scene construction
  void build_scene_1();
  void build_scene_2();
  void build_scene_3();

  //Treenode construction
  void treeview();
  void construst_tree();
  void construct_node_scene(vector<vector<tree_file*>>& nodes_path_vec);
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
  Transformation* transformManager;

  vector<tree_file*> nodes_root;
  vector<vector<tree_file*>> nodes_path_vec;
  vector<string> accepted_format;
  vector<string> path_init_vec;
  vector<string> path_init_file;
  bool with_remove_cloud;
  bool with_onthefly;
  float object_scale;
  string lidar_model;
};

#endif
