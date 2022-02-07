#ifndef GUI_LEFTPANEL_H
#define GUI_LEFTPANEL_H

class GUI_windows;
class GUI_module;
class GUI_fileManager;
class GUI_node;

class Engine;
class Dimension;

#include "../../common.h"


class GUI_leftPanel
{
public:
  //Constructor / Destructor
  GUI_leftPanel(GUI_node* node_gui);
  ~GUI_leftPanel();

public:
  //Main function
  void design_leftPanel();

  void panel_top();
  void panel_bot();
  void update_dimension();

  inline vec2 get_lbp_dim(){return dim_lbp;}
  inline vec2 get_ltp_dim(){return dim_ltp;}

private:
  Dimension* dimManager;

  GUI_windows* gui_winManager;
  GUI_module* gui_moduleManager;
  GUI_fileManager* gui_fileManager;

  vec2 dim_lbp;
  vec2 dim_ltp;
};

#endif
