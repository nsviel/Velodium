#ifndef PLAYER_H
#define PLAYER_H

#include "../../common.h"

class Node_operation;
class Scene;
class Timer;
class Online;
class Saver;
class Object;
class Loader;
class Configuration;


class Player
{
public:
  //Constructor / Destructor
  Player(Node_operation* node_ope);
  ~Player();

public:
  void update_configuration();
  void runtime();

  //Selection function
  void select_bySubsetID(Cloud* cloud, int ID_subset);
  void compute_wheel_selection(string direction);
  void compute_onthefly(Cloud* cloud);
  bool compute_range_limit(Cloud* cloud, int& ID_subset);

  //Player functions
  void player_start();
  void player_pause();
  void player_start_or_pause();
  void player_stop();
  void player_save(Cloud* cloud);
  void player_setFrequency(int frequency);
  void player_selectDirSave();

  inline int* get_frequency(){return &player_frequency;}
  inline bool* get_player_isrunning(){return &player_isrunning;}
  inline bool* get_player_ispaused(){return &player_ispaused;}
  inline bool* get_with_restart(){return &player_returnToZero;}
  inline string* get_player_saveas(){return &player_saveas;}
  inline string* get_player_mode(){return &player_mode;}

private:
  Scene* sceneManager;
  Loader* loadManager;
  Timer* timerManager;
  Online* onlineManager;
  Saver* saveManager;
  Object* objectManager;
  Configuration* configManager;

  string player_saveas;
  string player_mode;
  bool player_isrunning;
  bool player_ispaused;
  bool player_returnToZero;
  bool player_time_flag;
  int player_frequency;
};

#endif
