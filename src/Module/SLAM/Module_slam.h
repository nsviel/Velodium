#ifndef MODULE_SLAM_H
#define MODULE_SLAM_H

#include "../../common.h"

class Engine_node;
class Node_operation;
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

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Module_node* get_node_module(){return node_module;}
  inline SLAM* get_slamManager(){return slamManager;}
  inline GUI_Slam* get_gui_slam(){return gui_slam;}

private:
  Engine_node* node_engine;
  Node_operation* node_ope;
  Module_node* node_module;
  SLAM* slamManager;
  GUI_Slam* gui_slam;
};

#endif
