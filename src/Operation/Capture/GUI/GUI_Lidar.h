#ifndef GUI_LIDAR_H
#define GUI_LIDAR_H

#include "../../../common.h"

class Scala;
class Velodyne;
class Capture;
class Node_operation;


class GUI_Lidar
{
public:
  //Constructor / Destructor
  GUI_Lidar(Node_operation* node_ope);
  ~GUI_Lidar();

public:
  //Main function
  void design_Lidar();

  //Specific functions
  void design_Velodyne();
  void design_Scala();
  void state_watcher();

  //Velodyn functions
  void velo_state();
  void velo_capture();
  void velo_parameter();

  //Scala functions
  void scala_state();
  void scala_file();
  void scala_capture();
  void scala_parameter();

private:
  Scala* scalaManager;
  Velodyne* veloManager;
  Capture* captureManager;

  int item_width;
};

#endif
