#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "../../common.h"

class Dimension;

class GUI_windows;
class GUI_control;
class GUI_option;
class GUI_menuBar;
class GUI_leftPanel;
class GUI_consol;
class GUI_operation;
class Node_gui;


class GUI
{
public:
  //Constructor / Destructor
  GUI(Node_gui* node_gui);
  ~GUI();

public:
  //Main functions
  void Gui_init();
  void Gui_style();
  void Gui_font();
  void Gui_loop();
  void Gui_render();

  //Subfunctions
  void Gui_Dimensions();

private:
  Dimension* dimManager;
  GUI_operation* gui_operation;
  GUI_windows* gui_window;
  GUI_control* gui_control;
  GUI_option* gui_option;
  GUI_leftPanel* gui_leftPanel;
  GUI_menuBar* gui_menuBar;
  GUI_consol* gui_consol;

  bool is_visualization;
};

#endif
