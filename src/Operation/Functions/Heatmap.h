#ifndef HEATMAP_H
#define HEATMAP_H

class Scene;
class Attribut;

#include "../../common.h"

class Heatmap
{
public:
  //Constructor / Destructor
  Heatmap();
  ~Heatmap();

public:
  //Main functions
  void set_Heatmap_all(bool heatAll);
  void set_Heatmap(Cloud* cloud);
  void set_Heatmap(Cloud* cloud, bool is_heatmap);
  void set_HeatmapField(int value);

  //Heatmap functions
  void compute_subset_heatmap_ON(Subset* subset);
  void compute_subset_heatmap_OFF(Subset* subset);
  void compute_heatmap_color(Subset* subset, vector<float>& v_in);

  //Plot functions
  void plot_colorPalette(Subset* subset);

  //Setters / Getters
  inline bool* get_param_Normalized(){ return &normalized;}
  inline vec2* get_heatmap_range(){return &range;}

private:
  Scene* sceneManager;
  Attribut* attribManager;

  int HMmode;
  bool normalized;
  vec2 range;
};

#endif
