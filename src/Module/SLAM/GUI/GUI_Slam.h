#ifndef GUI_SLAM_H
#define GUI_SLAM_H

#include "../../../common.h"

class Engine;
class Scene;
class CT_ICP;

class SLAM_optim_ceres;
class SLAM_optim_gn;
class SLAM_normal;
class SLAM_assessment;
class SLAM_localMap;


class GUI_Slam
{
public:
  //Constructor / Destructor
  GUI_Slam(Engine* engineManager);
  ~GUI_Slam();

public:
  void design_SLAM();

  void compute();
  void statistics();

  void parameters();
  void parameters_general();
  void parameters_optimization();
  void parameters_localMap();
  void parameters_normal();
  void parameters_robustesse();

  vec3 compute_anglesFromTransformationMatrix(const mat4& mat);

  inline CT_ICP* get_cticpManager(){return cticpManager;}

private:
  Scene* sceneManager;
  CT_ICP* cticpManager;

  SLAM_optim_ceres* ceresManager;
  SLAM_optim_gn* gnManager;
  SLAM_normal* normalManager;
  SLAM_assessment* assessManager;
  SLAM_localMap* mapManager;

  int item_width;
};

#endif
