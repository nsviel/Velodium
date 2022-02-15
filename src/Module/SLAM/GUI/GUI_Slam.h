#ifndef GUI_SLAM_H
#define GUI_SLAM_H

#include "../../../common.h"

class GUI_node;
class Scene;
class Slam;

class SLAM_optim_ceres;
class SLAM_optim_gn;
class SLAM_normal;
class SLAM_assessment;
class SLAM_localMap;
class SLAM_configuration;


class GUI_Slam
{
public:
  //Constructor / Destructor
  GUI_Slam(GUI_node* node_gui);
  ~GUI_Slam();

public:
  void design_SLAM();

  void compute();
  void statistics();

  void parameter_slam();
  void parameter_configuration();
  void parameter_general();
  void parameter_optimization();
  void parameter_localMap();
  void parameter_normal();
  void parameter_robustesse();

  vec3 compute_anglesFromTransformationMatrix(const mat4& mat);

  inline Slam* get_slamManager(){return slamManager;}

private:
  Scene* sceneManager;
  Slam* slamManager;

  SLAM_optim_ceres* ceresManager;
  SLAM_optim_gn* gnManager;
  SLAM_normal* normalManager;
  SLAM_assessment* assessManager;
  SLAM_localMap* mapManager;
  SLAM_configuration* configManager;

  int item_width;
};

#endif
