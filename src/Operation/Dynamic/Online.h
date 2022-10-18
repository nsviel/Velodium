#ifndef ONLINE_H
#define ONLINE_H

#include "../../common.h"

class Node_operation;
class SLAM;
class Color;
class Camera;
class Engine;
class Filter;
class Dimension;
class Scene;
class Configuration;
class Followup;
class Interface;
class Object;
class Renderer;
class http_command;


class Online
{
public:
  //Constructor / Destructor
  Online(Node_operation* node_ope);
  ~Online();

public:
  //Main functions
  void update_configuration();
  void compute_onlineOpe(Cloud* cloud, int ID_subset);

  //Other functions
  void compute_displayStats(Subset* subset);
  void compute_visibility(Cloud* cloud, int& ID_subset);
  void compute_http_command();

  inline bool* get_with_slam(){return &with_slam;}
  inline bool* get_with_cylinder_filter(){return &with_cylinder_cleaning;}
  inline int* get_visibility_range(){return &visibility_range;}
  inline float get_time_operation(){return time_operation;}

private:
  Scene* sceneManager;
  Color* colorManager;
  SLAM* slamManager;
  Filter* filterManager;
  Dimension* dimManager;
  Configuration* configManager;
  Followup* followManager;
  Object* objectManager;
  Renderer* renderManager;
  http_command* commandManager;

  float time_operation;
  int visibility_range;
  bool with_subset_specific_color;
  bool with_slam;
  bool with_cylinder_cleaning;
  bool with_remove_lastSubset;
};

#endif
