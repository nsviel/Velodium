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
class Interface;
class Object;
class Renderer;
class http_command;
class Visibility;


class Online
{
public:
  //Constructor / Destructor
  Online(Node_operation* node_ope);
  ~Online();

public:
  //Main functions
  void update_configuration();
  void compute_onlineOpe(Collection* collection, int ID_object);

  //Other functions
  void compute_recording(Collection* collection, int& ID_object);
  void compute_displayStats(Object_* object);
  void compute_http_command();

  inline bool* get_with_sphere_filter(){return &with_filter_sphere;}
  inline bool* get_with_slam(){return &with_slam;}
  inline bool* get_with_camera_follow(){return &with_camera_follow;}
  inline float get_time_operation(){return time_ope;}
  inline int* get_filter_mode(){return &filter_mode;}

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
  Visibility* visibilityManager;

  float time_ope;
  int filter_mode;
  bool with_subset_specific_color;
  bool with_filter_sphere;
  bool with_remove_lastSubset;
  bool with_slam = true;
  bool with_camera_follow = true;
};

#endif
