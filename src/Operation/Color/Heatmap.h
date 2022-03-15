#ifndef HEATMAP_H
#define HEATMAP_H

class Operation_node;
class Scene;
class Attribut;

#include "gnuplot/gnuplot-iostream.h"

#include "../../common.h"


class Heatmap
{
public:
  //Constructor / Destructor
  Heatmap(Operation_node* node_ope);
  ~Heatmap();

public:
  //Main functions
  void make_cloud_heatmap(Cloud* cloud);
  void make_subset_heatmap(Subset* subset);
  void make_heatmap_all(bool heatAll);

  //Specific mode functions
  void mode_height(Subset* subset);
  void mode_intensity(Subset* subset);
  void mode_distance(Subset* subset);
  void mode_cosIt(Subset* subset);
  void mode_It(Subset* subset);

  //Heatmap functions
  void set_heatmap_mode(Subset* subset);
  void set_heatmap(Subset* subset, vector<float>& v_in);
  void unset_heatmap(Subset* subset);

  //Plot functions
  void plot_colorPalette(Subset* subset);

  //Setters / Getters
  inline int* get_heatmap_mode(){return &heatmap_mode;}
  inline bool* get_is_normalization(){ return &is_normalization;}
  inline vec2* get_range_normalization(){return &range_norm;}
  inline vec2* get_range_height(){return &range_height;}
  inline vec2* get_range_intensity(){return &range_intensity;}

private:
  Scene* sceneManager;
  Attribut* attribManager;

  vec2 range_norm;
  vec2 range_height;
  vec2 range_intensity;
  
  bool is_normalization;
  int heatmap_mode;
};

#endif