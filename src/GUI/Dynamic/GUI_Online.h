#ifndef GUI_ONLINE_H
#define GUI_ONLINE_H

#include "../../common.h"

class Node_engine;
class Node_module;
class Node_gui;

class Scene;
class Player_cloud;
class Filter;
class Online;
class GUI_Color;
class GUI_Player;
class Followup;
class Player;
class Saving;
class Renderer;
class Color;
class Configuration;


class GUI_Online
{
public:
  //Constructor / Destructor
  GUI_Online(Node_gui* node_ope);
  ~GUI_Online();

public:
  //Main function
  void design_dynamic();
  void design_state();
  void design_online();

  //Parameter function
  void parameter_offline();
  void parameter_online();
  void parameter_export();

  //State function
  void state_time();
  void state_online();
  void state_configuration();

private:
  Node_module* node_module;
  Node_engine* node_engine;

  Followup* followManager;
  Scene* sceneManager;
  Filter* filterManager;
  Online* onlineManager;
  GUI_Color* gui_color;
  GUI_Player* gui_player;
  Saving* savingManager;
  Renderer* renderManager;
  Configuration* configManager;
  Color* colorManager;
  Player* playerManager;

  int item_width;
};

#endif
