#ifndef GUI_CLOUDPLAYER_H
#define GUI_CLOUDPLAYER_H

#include "../../common.h"

class Scene;
class Engine;
class CloudPlayer;
class Heatmap;
class Filter;
class Online;


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
  void parameter_offline();
  void parameter_online();

private:
  Scene* sceneManager;
  CloudPlayer* playerManager;
  Heatmap* heatmapManager;
  Filter* filterManager;
  Online* onlineManager;
};

#endif
