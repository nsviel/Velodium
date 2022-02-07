#ifndef GUI_LIDAR_H
#define GUI_LIDAR_H

class Scala;
class Velodyne;
class Capture;
class GUI_node;

#include "../../../common.h"


class GUI_Lidar
{
public:
  //Constructor / Destructor
  GUI_Lidar(GUI_node* node_gui);
  ~GUI_Lidar();

public:
  void design_Scala();
  //Main function
  void design_Velodyne();

  //Subfunctions
  void velo_state();
  void velo_connection();
  void velo_capture();
  void velo_parameter();

private:
  Scala* scalaManager;
  Velodyne* veloManager;
  Capture* captureManager;
};

#endif
