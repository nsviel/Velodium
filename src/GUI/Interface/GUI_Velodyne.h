#ifndef GUI_VELODYNE_H
#define GUI_VELODYNE_H

#include "../../common.h"

class Node_gui;
class Velodyne;
class Capture;


class GUI_Velodyne
{
public:
  //Constructor / Destructor
  GUI_Velodyne(Node_gui* node_gui);
  ~GUI_Velodyne();

public:
  //Main function
  void design_Velodyne();

  //Velodyn functions
  void velo_state();
  void velo_statistique();
  void velo_capture();
  void velo_parameter();

private:
  Velodyne* veloManager;
  Capture* captureManager;

  int item_width;
};

#endif
