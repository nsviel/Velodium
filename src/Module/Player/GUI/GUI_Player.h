#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../../common.h"

class Scene;
class Engine;
class Player_cloud;
class Heatmap;
class Filter;
class Player_online;


class GUI_Player
{
public:
  //Constructor / Destructor
  GUI_Player(Engine* engineManager);
  ~GUI_Player();

public:
  //Main function
  void design_player_cloud();
  void design_player_online();

  //Subfunctions
  void player_run();
  void player_mouse();
  void player_selection();
  void parameter_offline();
  void parameter_online();

private:
  Scene* sceneManager;
  Heatmap* heatmapManager;
  Filter* filterManager;
  Player_cloud* playerManager;
  Player_online* onlineManager;
};

#endif
