#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../common.h"

class GUI_module;
class GUI_Dynamic;
class GUI_Obstacle;
class GUI_State;


class GUI_Player
{
public:
  //Constructor / Destructor
  GUI_Player(GUI_module* gui_module);
  ~GUI_Player();

public:
  //Main function
  void design_player();
  void runtime();

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
