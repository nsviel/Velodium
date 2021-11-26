#ifndef GUI_SLAM_H
#define GUI_SLAM_H

#include "../../../common.h"

class CT_ICP;
class SLAM_optim_ceres;


class GUI_slam
{
public:
  //Constructor / Destructor
  GUI_slam();
  ~GUI_slam();

public:
  void design_SLAM();

private:
  CT_ICP* cticpManager;
  SLAM_optim_ceres* slam_optiManager;
};

#endif
