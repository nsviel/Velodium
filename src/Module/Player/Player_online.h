#ifndef PLAYER_ONLINE_H
#define PLAYER_ONLINE_H

#include "../../common.h"

class CT_ICP;
class Heatmap;
class Camera;
class Engine;
class Filter;
class Dimension;
class Renderer;
class Scene;
class Obstacle;
class config_module;
class Module_node;


class Player_online
{
public:
  //Constructor / Destructor
  Player_online(Module_node* node_module);
  ~Player_online();

public:
  void compute_onlineOpe(Cloud* cloud, int ID_subset);
  void update_configuration();

  //Camera functions
  void camera_followUp(Cloud* cloud, int i);
  void camera_position(Subset* subset);
  void camera_orientation(Subset* subset);

  //Save functions
  void save_image(Subset* subset);
  void save_image_path();

  //Other functions
  void color_heatmap(Cloud* cloud, int i);
  void color_unicolor(Subset* subset, vec4 color);
  void remove_subset_last(Cloud* cloud, int i);

  inline bool* get_with_camera_follow(){return &with_camera_follow;}
  inline bool* get_with_slam(){return &with_slam;}
  inline bool* get_with_cylinder_filter(){return &with_cylinder_cleaning;}
  inline bool* get_with_heatmap(){return &with_heatmap;}
  inline bool* get_with_heatmap_rltHeight(){return &with_heatmap_rltHeight;}
  inline bool* get_with_unicolor(){return &with_unicolor;}
  inline bool* get_with_save_image(){return &with_save_image;}
  inline bool* get_with_online(){return &with_online;}
  inline vec2* get_heatmap_height_range(){return &HM_height_range;}
  inline bool* get_with_keepNframes(){return &with_keepNframes;}
  inline bool* get_with_remove_lastSubset(){return &with_remove_lastSubset;}
  inline string* get_save_image_path(){return &screenshot_path;}
  inline int* get_nb_subset_max(){return &nb_subset_max;}

private:
  Scene* sceneManager;
  Heatmap* heatmapManager;
  Camera* cameraManager;
  CT_ICP* cticpManager;
  Filter* filterManager;
  Dimension* dimManager;
  Renderer* renderManager;
  Obstacle* obstacleManager;
  config_module* configManager;

  string screenshot_path;
  vec2 HM_height_range;
  vec2 camera_moved_trans;
  float camera_moved_rotat;
  float camera_distPos;
  int nb_subset_max;

  bool with_online;
  bool with_camera_follow;
  bool with_slam;
  bool with_camera_top;
  bool with_camera_root;
  bool with_cylinder_cleaning;
  bool with_heatmap;
  bool with_heatmap_rltHeight;
  bool with_unicolor;
  bool with_save_image;
  bool with_keepNframes;
  bool with_remove_lastSubset;
  bool with_AI_module;
};

#endif
