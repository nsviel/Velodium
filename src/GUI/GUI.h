#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "../common.h"

class Dimension;

class GUI_windows;
class GUI_control;
class GUI_option;
class GUI_menuBar;
class GUI_leftPanel;
class GUI_consol;
class GUI_module;
class GUI_node;


class GUI
{
public:
  //Constructor / Destructor
  GUI(GUI_node* node_gui);
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
  GUI_module* gui_moduleManager;
  GUI_windows* gui_winManager;
  GUI_control* gui_controlManager;
  GUI_option* gui_optionManager;
  GUI_leftPanel* gui_leftPanelManager;
  GUI_menuBar* gui_menuBarManager;
  GUI_consol* gui_consolManager;

  bool is_visualization;
};

#endif
