#ifndef WIN_ATTRIBUT_H
#define WIN_ATTRIBUT_H

#include "../../common.h"

class Scene;
class Attribut;
class Transforms;
class Glyphs;
class Heatmap;
class Node_gui;
class GUI_Color;
class Color;


class WIN_attribut
{
public:
  //Constructor / Destructor
  WIN_attribut(Node_gui* node_gui);
  ~WIN_attribut();

public:
  void window_normal();
  void window_intensity();
  void window_color();

private:
  GUI_Color* gui_color;
  Scene* sceneManager;
  Attribut* attribManager;
  Color* colorManager;
  Transforms* transformManager;
  Glyphs* glyphManager;
  Heatmap* heatmapManager;

  int item_width;
};

#endif
