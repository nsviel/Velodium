#ifndef GUI_PLAYER_H
#define GUI_PLAYER_H

#include "../../../common.h"

class Node_operation;
class Scene;
class Player;
class Online;
class Capture;


class GUI_Player
{
public:
  //Constructor / Destructor
  GUI_Player(Node_operation* node_ope);
  ~GUI_Player();

public:
  //Main function
  void runtime_player_mouse();

  //Subfunctions
  void player_run();
  void player_button();
  void player_selection();

  //Specific button function
  void button_offline_play(Cloud* cloud);
  void button_offline_pause(Cloud* cloud);
  void button_offline_stop(Cloud* cloud);

  void button_online_play(Cloud* cloud);
  void button_online_pause(Cloud* cloud);
  void button_online_stop(Cloud* cloud);

private:
  Scene* sceneManager;
  Player* playerManager;
  Online* onlineManager;
  Capture* captureManager;

  int item_width;
};

#endif
