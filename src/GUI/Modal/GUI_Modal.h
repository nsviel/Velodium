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
class MOD_transformation;
class MOD_filter;
class MOD_extraction;
class MOD_boxing;
class MOD_texture;


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
  MOD_camera* mod_camera;
  MOD_shader* mod_shader;
  MOD_loading* mod_loading;
  MOD_cloud* mod_cloud;
  MOD_operation* mod_operation;
  MOD_attribut* mod_attribut;
  MOD_transformation* mod_transformation;
  MOD_filter* mod_filter;
  MOD_extraction* mod_extraction;
  MOD_boxing* mod_boxing;
  MOD_texture* mod_texture;
};

#endif
