#ifndef MODULE_SLAM_H
#define MODULE_SLAM_H

#include "../../common.h"

class Node_engine;
class Node_operation;
class Node_module;

class SLAM;
class SLAM_parameter;
class GUI_Slam;
class CT_ICP;


class Module_slam
{
public:
  //Constructor / Destructor
  Module_slam(Node_module* node);
  ~Module_slam();

public:
  //Main functions
  void update();
  void reset();
  void draw();
  void draw_online();
  void online(Collection* collection, int subset_ID);

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Node_module* get_node_module(){return node_module;}

  inline SLAM* get_slamManager(){return slamManager;}
  inline SLAM_parameter* get_slam_param(){return slam_param;}
  inline GUI_Slam* get_gui_slam(){return gui_slam;}
  inline CT_ICP* get_cticpManager(){return cticpManager;}

  inline bool* get_with_slam(){return &with_slam;}
  inline int* get_algo(){return &algo;}

private:
  Node_engine* node_engine;
  Node_operation* node_ope;
  Node_module* node_module;

  SLAM* slamManager;
  CT_ICP* cticpManager;
  SLAM_parameter* slam_param;
  GUI_Slam* gui_slam;

  bool with_slam;
  int algo;
};

#endif
