#ifndef GUI_CAPTURE_H
#define GUI_CAPTURE_H

#include "../../../common.h"

class Scala;
class Velodyne;
class Capture;
class Node_interface;


class GUI_Capture
{
public:
  //Constructor / Destructor
  GUI_Capture(Node_interface* node_interface);
  ~GUI_Capture();

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
