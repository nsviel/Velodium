#ifndef GUI_OPION_H
#define GUI_OPION_H

class GUI_Control;
class Node_gui;

class Scene;
class Pather;
class Object;
class Color;
class Heatmap;
class Transformation;
class Renderer;
class Configuration;

#include "../../common.h"


class GUI_option
{
public:
  //Constructor / Destructor
  GUI_option(Node_gui* node_gui);
  ~GUI_option();

public:
  //Main function
  void design_Options();

  //Subfunctions
  void option_color();
  void option_glyph();
  void option_parameter();
  void option_mode();
  void option_font();

  inline void set_backgroundColorPtr(vec3* value){this->backgColor = value;}

private:
  GUI_Control* gui_control;
  GUI_option* gui_option;

  Configuration* configManager;
  Scene* sceneManager;
  Color* colorManager;
  Transformation* transformManager;
  Object* objectManager;
  Heatmap* heatmapManager;
  Pather* pathManager;
  Renderer* renderManager;

  vec3* backgColor;
};

#endif
