#ifndef HEATMAP_H
#define HEATMAP_H

#include "../../common.h"

class Scene;
class Attribut;
class Colormap;
class GPU_data;


class Heatmap
{
public:
  //Constructor / Destructor
  Heatmap();
  ~Heatmap();

public:
  //Main functions
  void make_col_heatmap(Collection* collection);
  void make_cloud_heatmap(Cloud* cloud);
  void make_heatmap_all(bool heatAll);

  //Specific mode functions
  void mode_height(Cloud* cloud);
  void mode_intensity(Cloud* cloud);
  void mode_distance(Cloud* cloud);
  void mode_cosIt(Cloud* cloud);
  void mode_It(Cloud* cloud);

  //Heatmap functions
  void heatmap_set(Cloud* cloud, vector<float>& v_in);
  void heatmap_unset(Cloud* cloud);

  //Setters / Getters
  inline int* get_heatmap_mode(){return &heatmap_mode;}
  inline bool* get_is_normalization(){ return &is_normalization;}
  inline vec2* get_range_normalization(){return &range_norm;}
  inline vec2* get_range_height(){return &range_height;}
  inline vec2* get_range_intensity(){return &range_intensity;}
  inline Colormap* get_colormapManager(){return colormapManager;}

private:
  Scene* sceneManager;
  Attribut* attribManager;
  Colormap* colormapManager;
  GPU_data* gpuManager;

  vec2 range_norm;
  vec2 range_height;
  vec2 range_intensity;
  bool is_normalization;
  int heatmap_mode;
};

#endif
