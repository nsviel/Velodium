#ifndef GUI_LIDAR_H
#define GUI_LIDAR_H

class Scala;
class Velodyne;
class Capture;

#include "../../../common.h"


class GUI_Lidar
{
public:
  //Constructor / Destructor
  GUI_Lidar();
  ~GUI_Lidar();

public:
  void design_Scala();
  //Main function
  void design_Velodyne();

  //Subfunctions
  void lidar_State();
  void lidar_Parameters();
  void lidar_Capture();
  void lidar_Recording();

private:
  Scala* scalaManager;
  Velodyne* veloManager;
  Capture* captureManager;
};

#endif
