#ifndef GUI_RECORDER_H
#define GUI_RECORDER_H

#include "../../common.h"

class Node_gui;
class Recorder;


class GUI_Recorder
{
public:
  //Constructor / Destructor
  GUI_Recorder(Node_gui* node_gui);
  ~GUI_Recorder();

public:
  //Main function
  void design_recorder();

  //Specific functions
  void recorder_screenshot();
  void recorder_frame();

private:
  Recorder* recordManager;

  int item_width;
};

#endif
