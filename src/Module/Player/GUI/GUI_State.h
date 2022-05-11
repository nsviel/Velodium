#ifndef GUI_STATE_H
#define GUI_STATE_H

#include "../../../common.h"

class GUI_Interface;
class GUI_module;

class Engine_node;
class Player_node;
class Configuration;
class Color;


class GUI_State
{
public:
  //Constructor / Destructor
  GUI_State(GUI_module* gui_module);
  ~GUI_State();

public:
  //Main function
  void design_state();

  //Subfunctions
  void state_configuration();
  void state_online();

private:
  GUI_Interface* gui_interface;

  Engine_node* node_engine;
  Player_node* node_player;
  Configuration* configManager;
  Color* colorManager;

  int item_width;
};

#endif
