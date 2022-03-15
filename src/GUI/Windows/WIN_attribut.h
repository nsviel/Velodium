#ifndef WIN_ATTRIBUT_H
#define WIN_ATTRIBUT_H

#include "../../common.h"

class Scene;
class Attribut;
class Transforms;
class Glyphs;
class Heatmap;
class Operation_node;
class Color;


class WIN_attribut
{
public:
  //Constructor / Destructor
  WIN_attribut(Operation_node* node_ope);
  ~WIN_attribut();

public:
  void window_normal();
  void window_intensity();
  void window_color();
  void window_heatmap();

private:
  Scene* sceneManager;
  Attribut* attribManager;
  Color* colorManager;
  Transforms* transformManager;
  Glyphs* glyphManager;
  Heatmap* heatmapManager;

  int item_width;
};

#endif
