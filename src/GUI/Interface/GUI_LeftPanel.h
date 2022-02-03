#ifndef GUI_LEFTPANEL_H
#define GUI_LEFTPANEL_H

class GUI_windows;
class GUI_Module;
class GUI_fileManager;

class Engine;
class Dimension;

#include "../../common.h"


class GUI_leftPanel
{
public:
  //Constructor / Destructor
  GUI_leftPanel(Engine* renderer, GUI_windows* window);
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
  GUI_windows* gui_winManager;
  GUI_Module* gui_moduleManager;
  GUI_fileManager* gui_fileManager;

  Engine* engineManager;
  Dimension* dimManager;

  vec2 dim_lbp;
  vec2 dim_ltp;
};

#endif
