#ifndef HEATMAP_H
#define HEATMAP_H

class Scene;
class Attribut;

#include "gnuplot/gnuplot-iostream.h"

#include "../../common.h"

class Heatmap
{
public:
  //Constructor / Destructor
  Heatmap();
  ~Heatmap();

public:
  //Main functions
  void set_Heatmap(Cloud* cloud);
  void set_Heatmap(Cloud* cloud, bool is_heatmap);
  void set_Heatmap(Subset* subset, Subset* subset_buffer, bool is_heatmap);
  void set_Heatmap_all(bool heatAll);

  //Heatmap functions
  void compute_subset_heatmap_ON(Subset* subset);
  void compute_subset_heatmap_OFF(Subset* subset);
  void compute_heatmap_color(Subset* subset, vector<float>& v_in);

  //Plot functions
  void plot_colorPalette(Subset* subset);

  //Setters / Getters
  inline bool* get_param_Normalized(){ return &normalized;}
  inline vec2* get_heatmap_range(){return &range_norm;}
  inline vec2* get_height_range(){return &range_height;}
  inline int* get_HeatmapField(){return &HMmode;}

private:
  Scene* sceneManager;
  Attribut* attribManager;

  int HMmode;
  bool normalized;
  vec2 range_norm, range_height;
};

#endif
