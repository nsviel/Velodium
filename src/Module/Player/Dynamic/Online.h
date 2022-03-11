#ifndef ONLINE_H
#define ONLINE_H

#include "../../../common.h"

class Slam;
class Heatmap;
class Camera;
class Engine;
class Filter;
class Dimension;
class Scene;
class Configuration;
class Interface_node;
class Module_node;


class Online
{
public:
  //Constructor / Destructor
  Online(Module_node* node_module);
  ~Online();

public:
  void compute_onlineOpe(Cloud* cloud, int ID_subset);
  void compute_displayStats(Subset* subset);
  void update_configuration();

  //Camera functions
  void camera_followUp(Cloud* cloud, int i);
  void camera_position(Subset* subset);
  void camera_orientation(Subset* subset);

  //Other functions
  void cloud_size_controler(Cloud* cloud);
  void set_cloud_visibility(Cloud* cloud, int& ID_subset);

  //Colorization
  void color_heatmap(Cloud* cloud, int i);
  void color_intensity(Subset* subset);
  void color_unicolor(Subset* subset, vec4 color);

  inline bool* get_with_camera_follow(){return &with_camera_follow;}
  inline bool* get_with_slam(){return &with_slam;}
  inline bool* get_with_cylinder_filter(){return &with_cylinder_cleaning;}
  inline bool* get_with_heatmap(){return &with_heatmap;}
  inline bool* get_with_heatmap_rltHeight(){return &with_heatmap_rltHeight;}
  inline bool* get_with_intensity(){return &with_intensity;}
  inline bool* get_with_unicolor(){return &with_unicolor;}
  inline bool* get_with_save_frame(){return &with_save_frame;}
  inline bool* get_with_save_image(){return &with_save_image;}

  inline vec2* get_heatmap_height_range(){return &HM_height_range;}
  inline bool* get_with_remove_lastSubset(){return &with_remove_lastSubset;}
  inline int* get_visibility_range(){return &visibility_range;}
  inline int* get_nb_subset_max(){return &nb_subset_max;}

private:
  Scene* sceneManager;
  Heatmap* heatmapManager;
  Camera* cameraManager;
  Slam* slamManager;
  Filter* filterManager;
  Dimension* dimManager;
  Interface_node* node_interface;
  Configuration* configManager;

  vec2 HM_height_range;
  vec2 camera_moved_trans;
  float camera_moved_rotat;
  float camera_distPos;
  int visibility_range;
  int nb_subset_max;

  bool with_justOneFrame;
  bool with_camera_follow;
  bool with_slam;
  bool with_camera_top;
  bool with_camera_root;
  bool with_cylinder_cleaning;
  bool with_heatmap;
  bool with_heatmap_rltHeight;
  bool with_intensity;
  bool with_unicolor;
  bool with_remove_lastSubset;
  bool with_save_frame;
  bool with_save_image;
};

#endif
