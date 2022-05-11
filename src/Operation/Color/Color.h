#ifndef COLOR_H
#define COLOR_H

#include "../../common.h"

class Operation_node;
class Scene;
class Heatmap;
class Configuration;


class Color
{
public:
  //Constructor / Destructor
  Color(Operation_node* node_ope);
  ~Color();

public:
  void update_configuration();

  //Colorization functions
  void make_colorization(Cloud* cloud, int ID_subset);
  void make_colorization(Subset* subset, vec4 RGB_in);
  void make_colorization_specific(Subset* subset);

  //Specific functions
  void color_unicolor(Subset* subset, vec4 color);
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
  inline vec2* get_range_intensity(){return &range_intensity;}

private:
  Scene* sceneManager;
  Heatmap* heatmapManager;
  Configuration* configManager;

  vec2 range_intensity;
  vec4 specific_color;
  int color_mode;
};

#endif
