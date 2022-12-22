#ifndef GUI_CAPTURE_H
#define GUI_CAPTURE_H

#include "../../common.h"

class Node_gui;
class GUI_Network;
class GUI_Scala;
class GUI_Velodyne;
class GUI_Recorder;


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
  void state_watcher();

private:
  GUI_Scala* gui_scala;
  GUI_Velodyne* gui_velodyne;
  GUI_Network* gui_network;
  GUI_Recorder* gui_recorder;

  int item_width;
};

#endif
