#ifndef WIN_HEATMAP_H
#define WIN_HEATMAP_H

#include "../../common.h"

class Scene;
class Heatmap;


class WIN_heatmap
{
public:
  //Constructor / Destructor
  WIN_heatmap(Heatmap* heat);
  ~WIN_heatmap();

public:
  void window_heatmap();

private:
  Heatmap* heatmapManager;
  Scene* sceneManager;
};

#endif
