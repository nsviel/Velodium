#ifndef GUI_COLOR_H
#define GUI_COLOR_H

#include "../../common.h"

class Node_gui;

class Scene;
class Heatmap;
class Color;
class Colormap;


class GUI_Color
{
public:
  //Constructor / Destructor
  GUI_Color(Node_gui* node_ope);
  ~GUI_Color();

public:
  //Main function
  void colorization_choice();

  //Option functions
  void option_intensity();
  void option_heatmap();
  void heatmap_select_colormap();
  void heatmap_application();
  void heatmap_mode();
  void heatmap_mode_height();
  void heatmap_mode_intensity();

private:
  Scene* sceneManager;
  Color* colorManager;
  Heatmap* heatmapManager;
  Colormap* colormapManager;

  int item_width;
};

#endif
