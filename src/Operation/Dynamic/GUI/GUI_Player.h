#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../common.h"

class GUI_operation;
class GUI_node;
class GUI_module;
class GUI_Dynamic;
class GUI_Obstacle;
class GUI_State;


class GUI_Player
{
public:
  //Constructor / Destructor
  GUI_Player(GUI_operation* node_gui);
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
  GUI_Dynamic* gui_dynamic;
  GUI_Obstacle* gui_obstacle;
  GUI_State* gui_state;

  int item_width;
};

#endif
