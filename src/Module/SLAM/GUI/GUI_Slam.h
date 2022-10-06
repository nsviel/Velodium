#ifndef GUI_SLAM_H
#define GUI_SLAM_H

#include "../../../common.h"

class GUI_module;
class Scene;
class Object;
class SLAM;

class SLAM_optim_ceres;
class SLAM_optim_gn;
class SLAM_normal;
class SLAM_assessment;
class SLAM_map;
class SLAM_parameter;


class GUI_Slam
{
public:
  //Constructor / Destructor
  GUI_Slam(GUI_module* node_gui);
  ~GUI_Slam();

public:
  void design_SLAM();

  void parameter_offline();
  void parameter_slam();
  void parameter_lidar();
  void parameter_glyph();
  void parameter_optimization();
  void parameter_localMap();
  void parameter_normal();
  void parameter_robustesse();

  void statistics();
  void display_stat(string title, vec3 abs, vec3 rlt);
  vec3 compute_anglesFromTransformationMatrix(const mat4& mat);

  inline SLAM* get_slamManager(){return slamManager;}

private:
  Scene* sceneManager;
  Object* objectManager;
  SLAM* slamManager;

  SLAM_optim_ceres* ceresManager;
  SLAM_optim_gn* gnManager;
  SLAM_normal* normalManager;
  SLAM_assessment* assessManager;
  SLAM_map* mapManager;
  SLAM_parameter* paramManager;

  int item_width;
};

#endif
