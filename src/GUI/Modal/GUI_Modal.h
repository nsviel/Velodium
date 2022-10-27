#ifndef GUI_MODAL_H
#define GUI_MODAL_H

#include "../../common.h"

class Node_gui;
class MOD_cloud;
class MOD_loading;
class MOD_camera;
class MOD_shader;
class MOD_operation;
class MOD_attribut;


class GUI_Modal
{
public:
  //Constructor / Destructor
  GUI_Modal(Node_gui* node_gui);
  ~GUI_Modal();

public:
  //Main functions
  void window_init();
  void window_Draw();

private:
  MOD_camera* win_camera;
  MOD_shader* win_shader;
  MOD_loading* win_loading;
  MOD_cloud* win_cloud;
  MOD_operation* win_operation;
  MOD_attribut* win_attribut;
};

#endif
