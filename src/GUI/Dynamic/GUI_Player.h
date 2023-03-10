#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../common.h"

class Node_gui;
class Node_interface;
class Scene;
class Player;
class Online;
class Visibility;


class GUI_Player
{
public:
  //Constructor / Destructor
  GUI_Player(Node_gui* node_gui);
  ~GUI_Player();

public:
  //Main function
  void design_player();
  void design_run();

  //Player action
  void player_recording();
  void player_visibility();
  void player_onthefly();
  void player_parameter();
  void player_button();
  void player_selection();
  void player_pause();

  //Specific button function
  void button_player_play(Collection* collection);
  void button_player_pause(Collection* collection);
  void button_player_stop(Collection* collection);

  void button_capture_play(Collection* collection);
  void button_capture_pause(Collection* collection);
  void button_capture_stop(Collection* collection);
  void button_capture_port();

private:
  Node_interface* node_interface;
  Scene* sceneManager;
  Player* playerManager;
  Online* onlineManager;
  Visibility* visibilityManager;

  int item_width;
};

#endif
