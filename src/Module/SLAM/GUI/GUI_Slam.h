#ifndef GUI_SLAM_H
#define GUI_SLAM_H

#include "../../../common.h"

class Module_slam;
class Scene;
class Object;
class SLAM;

class SLAM_optim_ceres;
class SLAM_optim_gn;
class SLAM_normal;
class SLAM_assessment;
class SLAM_map;
class SLAM_parameter;
class SLAM_transform;


class GUI_Slam
{
public:
  //Constructor / Destructor
  GUI_Slam(Module_slam* module);
  ~GUI_Slam();

public:
  void design_SLAM();
  void design_state();
  void design_parameter();
  void design_option();

  void parameter_offline();
  void parameter_lidar();
  void parameter_glyph();
  void parameter_optimization();
  void parameter_localMap();
  void parameter_normal();
  void parameter_robustesse();

  void state_SLAM();
  void state_localmap();
  void state_transformation();
  void display_stat(string title, vec3 abs, vec3 rlt);
  vec3 compute_anglesFromTransformationMatrix(const mat4& mat);

  inline SLAM* get_slamManager(){return slamManager;}

private:
  Scene* sceneManager;
  Object* objectManager;
  SLAM* slamManager;

  SLAM_optim_ceres* slam_optim_ceres;
  SLAM_optim_gn* slam_optim_gn;
  SLAM_normal* slam_normal;
  SLAM_assessment* slam_assess;
  SLAM_map* slam_map;
  SLAM_parameter* slam_param;
  SLAM_transform* slam_transf;

  int item_width;
};

#endif
