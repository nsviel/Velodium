#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../../common.h"

class Scene;
class GUI_node;
class Player_cloud;
class Heatmap;
class Filter;
class Offline;
class Online;
class Interface;
class GUI_Interface;


class GUI_Player
{
public:
  //Constructor / Destructor
  GUI_Player(GUI_node* node_gui);
  ~GUI_Player();

public:
  //Main function
  void design_player_cloud();

  //Subfunctions
  void player_run();
  void player_button();
  void player_selection();
  void parameter_offline();
  void parameter_online();
  void runtime_player_mouse();

private:
  Scene* sceneManager;
  Heatmap* heatmapManager;
  Filter* filterManager;
  Interface* ioManager;
  Offline* offlineManager;
  Online* onlineManager;
  GUI_Interface* gui_ioManager;
  
  int item_width;
};

#endif
