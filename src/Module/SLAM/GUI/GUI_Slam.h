#ifndef GUI_SLAM_H
#define GUI_SLAM_H

#include "../Base/common.h"

class Module_slam;
class Scene;
class SLAM;
class CT_ICP;

class SLAM_optim_ceres;
class SLAM_optim_gn;
class SLAM_normal;
class SLAM_assessment;
class SLAM_map;
class SLAM_parameter;
class SLAM_transform;
class SLAM_glyph;
class SLAM_optim;
class SLAM_sampling;


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
  void design_misc();

  void parameter_offline();
  void parameter_algo();
  void parameter_lidar();
  void parameter_glyph();
  void parameter_optimization();
  void parameter_gridSampling();
  void parameter_localMap();
  void parameter_localCloud();
  void parameter_normal();
  void parameter_robustesse();

  void state_SLAM();
  void state_localmap();
  void state_transformation();
  void display_stat(string title, vec3 abs, vec3 rlt);
  vec3 compute_anglesFromTransformationMatrix(const mat4& mat);

  inline SLAM* get_slamManager(){return slamManager;}

private:
  Module_slam* module_slam;
  Scene* sceneManager;
  SLAM* slamManager;
  CT_ICP* cticpManager;

  SLAM_optim_ceres* slam_optim_ceres;
  SLAM_optim_gn* slam_optim_gn;
  SLAM_normal* slam_normal;
  SLAM_assessment* slam_assess;
  SLAM_map* slam_map;
  SLAM_parameter* slam_param;
  SLAM_transform* slam_transf;
  SLAM_glyph* slam_glyph;
  SLAM_optim* slam_optim;
  SLAM_sampling* slam_sampling;

  int item_width;
};

#endif
