#ifndef MOD_ATTRIBUT_H
#define MOD_ATTRIBUT_H

#include "../../../common.h"

class Scene;
class Attribut;
class Transformation;
class Glyphs;
class Heatmap;
class Node_gui;
class GUI_Color;
class Color;
class Pose;


class MOD_attribut
{
public:
  //Constructor / Destructor
  MOD_attribut(Node_gui* node_gui);
  ~MOD_attribut();

public:
  void window_normal();
  void window_intensity();
  void window_color();

private:
  Transformation* transformManager;
  Pose* poseManager;
  GUI_Color* gui_color;
  Scene* sceneManager;
  Attribut* attribManager;
  Color* colorManager;
  Glyphs* glyphManager;
  Heatmap* heatmapManager;

  int item_width;
};

#endif
