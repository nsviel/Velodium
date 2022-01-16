#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../../common.h"

class Scene;
class Engine;
class CloudPlayer;
class Heatmap;
class Filter;
class OnlinePlayer;


class GUI_Player
{
public:
  //Constructor / Destructor
  GUI_Player(Engine* engineManager);
  ~GUI_Player();

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
  OnlinePlayer* onlineManager;
};

#endif
