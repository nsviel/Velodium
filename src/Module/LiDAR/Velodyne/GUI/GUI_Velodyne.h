#ifndef GUI_VELODYNE_H
#define GUI_VELODYNE_H

class Velodyne;
class Capture;

#include "../../../../common.h"


class GUI_Velodyne
{
public:
  //Constructor / Destructor
  GUI_Velodyne();
  ~GUI_Velodyne();

public:
  //Main function
  void design_Velodyne();

  //Subfunctions
  void lidar_State();
  void lidar_Parameters();
  void lidar_Capture();
  void lidar_Recording();

private:
  Velodyne* veloManager;
  Capture* captureManager;

};

#endif
