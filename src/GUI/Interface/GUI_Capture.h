#ifndef GUI_CAPTURE_H
#define GUI_CAPTURE_H

#include "../../common.h"

class Node_gui;
class Scala;
class Velodyne;
class Capture;
class GUI_Network;


class GUI_Capture
{
public:
  //Constructor / Destructor
  GUI_Capture(Node_gui* node_gui);
  ~GUI_Capture();

public:
  //Main function
  void design_interface();
  void design_capture();

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
  GUI_Network* gui_network;

  int item_width;
};

#endif
