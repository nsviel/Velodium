#ifndef GUI_CLOUDPLAYER_H
#define GUI_CLOUDPLAYER_H

#include "../../common.h"

class Scene;
class Engine;
class CloudPlayer;
class Heatmap;
class Filter;


class GUI_CloudPlayer
{
public:
  //Constructor / Destructor
  GUI_CloudPlayer(Engine* engineManager);
  ~GUI_CloudPlayer();

public:
  //Main function
  void design_CloudPlayer();

  //Subfunctions
  void playCloud();
  void playCloud_byMouseWheel();
  void subset_selection_bar();
  void parameter();

private:
  Scene* sceneManager;
  CloudPlayer* playerManager;
  Heatmap* heatmapManager;
  Filter* filterManager;

};

#endif
