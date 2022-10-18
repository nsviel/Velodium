#ifndef MODULE_SLAM_H
#define MODULE_SLAM_H

#include "../../common.h"

class Module_node;
class SLAM;
class GUI_Slam;


class Module_slam
{
public:
  //Constructor / Destructor
  Module_slam(Module_node* node);
  ~Module_slam();

public:
  //Main functions
  void init();
  void update();
  void runtime();
  void reset();
  void draw();

private:
  SLAM* slamManager;
  GUI_Slam* gui_slam;
  Module_node* node_module;
};

#endif
