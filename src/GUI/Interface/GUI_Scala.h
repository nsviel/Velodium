#ifndef GUI_SCALA_H
#define GUI_SCALA_H

#include "../../common.h"

class Node_gui;
class Scala;
class Capture;


class GUI_Scala
{
public:
  //Constructor / Destructor
  GUI_Scala(Node_gui* node_gui);
  ~GUI_Scala();

public:
  //Main function
  void design_Scala();

  //Scala functions
  void scala_state();
  void scala_statistique();
  void scala_file();
  void scala_capture();
  void scala_parameter();

private:
  Scala* scalaManager;
  Capture* captureManager;

  int item_width;
};

#endif
