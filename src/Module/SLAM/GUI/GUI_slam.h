#ifndef GUI_SLAM_H
#define GUI_SLAM_H

#include "../../../common.h"

class CT_ICP;
class SLAM_optim_ceres;
class SLAM_optim_gn;


class GUI_slam
{
public:
  //Constructor / Destructor
  GUI_slam();
  ~GUI_slam();

public:
  void design_SLAM();

  void compute();
  void parameters();
  void statistics();

  vec3 compute_anglesFromTransformationMatrix(const mat4& mat);

private:
  CT_ICP* cticpManager;
  SLAM_optim_ceres* ceresManager;
  SLAM_optim_gn* gnManager;

  int item_width;
};

#endif
