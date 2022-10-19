#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../../common.h"

class Scene;
class Player;
class Online;
class Capture;
class Filter;
class Heatmap;
class Node_operation;
class Followup;

class GUI_node;
class GUI_module;
class GUI_Dynamic;
class GUI_Obstacle;
class GUI_State;
class GUI_Online;
class GUI_Color;


class GUI_Player
{
public:
  //Constructor / Destructor
  GUI_Player(Node_operation* node_ope);
  ~GUI_Player();

public:
  //Main function
  void design_player();
  void runtime_player_mouse();

  //Subfunctions
  void player_run();
  void player_button();
  void player_selection();
  void parameter_offline();
  void parameter_online();

  //Specific button functions
  void button_offline_play(Cloud* cloud);
  void button_offline_pause(Cloud* cloud);
  void button_offline_stop(Cloud* cloud);

  void button_online_play(Cloud* cloud);
  void button_online_pause(Cloud* cloud);
  void button_online_stop(Cloud* cloud);

  inline GUI_Dynamic* get_gui_dynamic(){return gui_dynamic;}
  inline GUI_Obstacle* get_gui_obstacle(){return gui_obstacle;}
  inline GUI_State* get_gui_state(){return gui_state;}

private:
  Scene* sceneManager;
  Player* playerManager;
  Online* onlineManager;
  Capture* captureManager;
  Filter* filterManager;
  Heatmap* heatmapManager;
  Followup* followManager;

  GUI_Dynamic* gui_dynamic;
  GUI_Obstacle* gui_obstacle;
  GUI_State* gui_state;
  GUI_Online* gui_online;
  GUI_Color* gui_color;

  int item_width;
};

#endif
