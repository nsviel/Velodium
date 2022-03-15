#ifndef COLOR_H
#define COLOR_H

#include "../../common.h"

class Operation_node;
class Scene;
class Heatmap;


class Color
{
public:
  //Constructor / Destructor
  Color(Operation_node* node_ope);
  ~Color();

public:
  //Color subset functions
  void make_colorization(Subset* subset);
  void color_unicolor(Subset* subset);
  void color_intensity(Subset* subset);
  void color_heatmap(Subset* subset);

  //Color cloud functions
  void set_color_new(Cloud* cloud, vec4 RGBA);
  void set_color_enhanced(Cloud* cloud);
  void set_color_random(Cloud* cloud);
  void set_color_initial(Cloud* cloud);
  void set_color_RGB(Cloud* cloud);
  void set_color_I(Cloud* cloud);
  string get_color_mode_name();

  inline int* get_color_mode(){return &color_mode;}

private:
  Scene* sceneManager;
  Heatmap* heatmapManager;

  int color_mode;
};

#endif
