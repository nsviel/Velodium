#ifndef COLOR_FUNCTION_H
#define COLOR_FUNCTION_H

#include "../../common.h"

class Node_operation;
class Scene;
class Heatmap;
class Configuration;


//Get a RGB random color
glm::vec4 random_color();


class Color
{
public:
  //Constructor / Destructor
  Color(Node_operation* node_ope);
  ~Color();

public:
  void update_configuration();

  //Colorization functions
  void make_colorization(Collection* collection, int ID_subset);
  void make_colorization(Cloud* subset, vec4 RGB_in);
  void make_colorization_specific(Cloud* subset);

  //Specific functions
  void color_unicolor(Cloud* subset, vec4 color);
  void color_intensity(Cloud* subset);
  void color_heatmap(Cloud* subset);

  //Color cloud functions
  void set_color_new(Collection* collection, vec4 RGBA);
  void set_color_enhanced(Collection* collection);
  void set_color_random(Collection* collection);
  void set_color_initial(Collection* collection);
  void set_color_RGB(Collection* collection);
  void set_color_I(Collection* collection);
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
