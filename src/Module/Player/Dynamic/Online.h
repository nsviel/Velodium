#ifndef ONLINE_H
#define ONLINE_H

#include "../../../common.h"

class Player_node;
class Module_node;

class Slam;
class Color;
class Camera;
class Engine;
class Filter;
class Dimension;
class Scene;
class Configuration;
class Saving;
class Capture;
class Followup;


class Online
{
public:
  //Constructor / Destructor
  Online(Player_node* node);
  ~Online();

public:
  //Main functions
  void update_configuration();
  void compute_onlineOpe(Cloud* cloud, int ID_subset);

  //Other functions
  void compute_displayStats(Subset* subset);
  void compute_colorization(Cloud* cloud, int ID_subset);
  void compute_size_controler(Cloud* cloud);
  void compute_visibility(Cloud* cloud, int& ID_subset);

  inline bool* get_with_slam(){return &with_slam;}
  inline bool* get_with_cylinder_filter(){return &with_cylinder_cleaning;}
  inline bool* get_with_save_frame(){return &with_save_frame;}
  inline bool* get_with_save_image(){return &with_save_image;}
  inline bool* get_with_remove_lastSubset(){return &with_remove_lastSubset;}
  inline int* get_visibility_range(){return &visibility_range;}
  inline int* get_nb_subset_max(){return &nb_subset_max;}

private:
  Scene* sceneManager;
  Color* colorManager;
  Slam* slamManager;
  Filter* filterManager;
  Dimension* dimManager;
  Configuration* configManager;
  Saving* saveManager;
  Capture* captureManager;
  Followup* followManager;

  int visibility_range;
  int nb_subset_max;

  bool with_justOneFrame;
  bool with_subset_specific_color;
  bool with_slam;
  bool with_cylinder_cleaning;
  bool with_remove_lastSubset;
  bool with_save_frame;
  bool with_save_image;
};

#endif
