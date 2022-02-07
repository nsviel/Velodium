#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../../common.h"

class Scene;
class GUI_node;
class Player_cloud;
class Heatmap;
class Filter;
class Player_online;


class GUI_Player
{
public:
  //Constructor / Destructor
  GUI_Player(GUI_node* node_gui);
  ~GUI_Player();

public:
  //Main function
  void design_player_cloud();
  void design_player_online();
  void update();

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
