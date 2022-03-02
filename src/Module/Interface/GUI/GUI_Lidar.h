#ifndef GUI_LIDAR_H
#define GUI_LIDAR_H

class Scala;
class Velodyne;
class Capture;
class GUI_module;

#include "../../../common.h"


class GUI_Lidar
{
public:
  //Constructor / Destructor
  GUI_Lidar(GUI_module* node_gui);
  ~GUI_Lidar();

public:
  //Main function
  void design_Velodyne();
  void design_Scala();

  //Velodyn functions
  void velo_state();
  void velo_connection();
  void velo_capture();
  void velo_parameter();

  //Scala functions
  void scala_state();
  void scala_file();
  void scala_capture();

private:
  Scala* scalaManager;
  Velodyne* veloManager;
  Capture* captureManager;

  int item_width;
};

#endif
