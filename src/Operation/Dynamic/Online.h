#ifndef ONLINE_H
#define ONLINE_H

#include "../../common.h"

class Node_operation;
class Node_engine;
class Color;
class Camera;
class Engine;
class Filter;
class Dimension;
class Scene;
class Configuration;
class Followup;
class Saving;
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

  //Visibility function
  void set_visibility_range(int value);
  int get_visibility_range_max();

  inline bool* get_with_cylinder_filter(){return &with_cylinder_cleaning;}
  inline float get_time_operation(){return time_operation;}
  inline int get_visibility_range(){return visibility_range;}

private:
  Node_engine* node_engine;
  Scene* sceneManager;
  Color* colorManager;
  Filter* filterManager;
  Dimension* dimManager;
  Configuration* configManager;
  Followup* followManager;
  Object* objectManager;
  Renderer* renderManager;
  http_command* httpManager;
  Saving* savingManager;

  float time_operation;
  int visibility_range;
  bool with_subset_specific_color;
  bool with_slam;
  bool with_cylinder_cleaning;
  bool with_remove_lastSubset;
};

#endif
