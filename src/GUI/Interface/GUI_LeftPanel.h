#ifndef GUI_LEFTPANEL_H
#define GUI_LEFTPANEL_H

class GUI_windows;
class GUI_module;
class GUI_fileManager;

class Engine;
class Dimension;

#include "../../common.h"

class GUI_leftPanel
{
public:
  //Constructor / Destructor
  GUI_leftPanel(Engine* renderer, GUI_windows* window, GUI_module* module);
  ~GUI_leftPanel();

public:
  //Main function
  void design_leftPanel();
  void panel_top();
  void panel_bot();

  inline float* get_panel_X(){return & panel_X;}

private:
  GUI_windows* gui_winManager;
  GUI_module* gui_moduleManager;
  GUI_fileManager* gui_fileManager;

  Engine* engineManager;
  Dimension* dimManager;

  ImVec2 XYmin, XYmax;
  float panel_Y;
  float panel_X;
  bool module_velodyne;
  bool module_odometry;
};

#endif
