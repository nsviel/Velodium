#ifndef GUI_CAPTURE_H
#define GUI_CAPTURE_H

#include "../../common.h"

class Node_gui;
class Capture;
class Recorder;
class GUI_Network;
class GUI_Scala;
class GUI_Velodyne;


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
  void design_recorder();

  //Specific functions
  void state_watcher();
  void recorder_screenshot();
  void recorder_frame();

private:
  Capture* captureManager;
  Recorder* recordManager;
  GUI_Scala* gui_scala;
  GUI_Velodyne* gui_velodyne;
  GUI_Network* gui_network;

  int item_width;
};

#endif
