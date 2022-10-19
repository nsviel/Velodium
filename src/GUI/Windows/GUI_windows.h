#ifndef GUI_WINDOWS_H
#define GUI_WINDOWS_H

#include "../../common.h"

class Node_gui;
class WIN_cloud;
class WIN_loading;
class WIN_camera;
class WIN_shader;
class WIN_operation;
class WIN_attribut;


class GUI_windows
{
public:
  //Constructor / Destructor
  GUI_windows(Node_gui* node_gui);
  ~GUI_windows();

public:
  //Main functions
  void window_init();
  void window_Draw();

private:
  WIN_camera* win_camera;
  WIN_shader* win_shader;
  WIN_loading* win_loading;
  WIN_cloud* win_cloud;
  WIN_operation* win_operation;
  WIN_attribut* win_attribut;
};

#endif
