#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../common.h"

class Node_gui;
class Node_interface;
class Scene;
class Player;
class Online;


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

  //Specific button function
  void button_player_play(Cloud* cloud);
  void button_player_pause(Cloud* cloud);
  void button_player_stop(Cloud* cloud);

  void button_capture_play(Cloud* cloud);
  void button_capture_pause(Cloud* cloud);
  void button_capture_stop(Cloud* cloud);
  void button_capture_port();

private:
  Node_interface* node_interface;
  Scene* sceneManager;
  Player* playerManager;
  Online* onlineManager;

  int item_width;
};

#endif
