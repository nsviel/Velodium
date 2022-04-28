#ifndef GUI_LIDAR_H
#define GUI_LIDAR_H

#include "../../../common.h"

class Scala;
class Velodyne;
class Capture;
class GUI_module;


class GUI_Lidar
{
public:
  //Constructor / Destructor
  GUI_Lidar(GUI_module* gui_module);
  ~GUI_Lidar();

public:
  //Main function
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
