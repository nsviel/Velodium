#ifndef GUI_OPION_H
#define GUI_OPION_H

class GUI_control;

class Scene;
class Engine;
class Operation;
class Glyphs;
class Attribut;
class Heatmap;
class Transforms;

#include "../../common.h"

class GUI_option
{
public:
  //Constructor / Destructor
  GUI_option(Engine* renderer, GUI_control* gui_controlManager);
  ~GUI_option();

public:
  //Main function
  void design_Options();

  //Subfunctions
  void option_colors();
  void option_glyphs();
  void option_heatmap();
  void option_parameters();
  void option_mode();
  void option_font();

  inline void set_backgroundColorPtr(vec3* value){this->backgColor = value;}

private:
  GUI_control* gui_controlManager;
  GUI_option* gui_optionManager;

  Scene* sceneManager;
  Engine* engineManager;
  Attribut* attribManager;
  Transforms* transformManager;
  Glyphs* glyphManager;
  Heatmap* heatmapManager;
  Operation* opeManager;

  vec3* backgColor;
};

#endif
