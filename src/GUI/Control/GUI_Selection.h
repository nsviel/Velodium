#ifndef GUI_SELECTION_H
#define GUI_SELECTION_H

#include "../../common.h"

class Node_gui;
class Selection;
class Dimension;


class GUI_Selection
{
public:
  //Constructor / Destructor
  GUI_Selection(Node_gui* node_gui);
  ~GUI_Selection();

public:
  void control_mouse();

private:
  Selection* selectionManager;
  Dimension* dimManager;

  int mouse_state;
};

#endif
