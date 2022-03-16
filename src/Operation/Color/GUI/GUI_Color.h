#ifndef GUI_COLOR_H
#define GUI_COLOR_H

#include "../../../common.h"

class Scene;
class GUI_node;
class Heatmap;
class Color;


class GUI_Color
{
public:
  //Constructor / Destructor
  GUI_Color(GUI_node* node_gui);
  ~GUI_Color();

public:
  void colorization_choice();
  void option_intensity();
  void option_heatmap();
  void heatmap();

private:
  Scene* sceneManager;
  Color* colorManager;
  Heatmap* heatmapManager;

  int item_width;
};

#endif
