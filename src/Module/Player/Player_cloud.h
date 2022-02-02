#ifndef PLAYER_CLOUD_H
#define PLAYER_CLOUD_H

#include "../../common.h"

class Scene;
class Timer;
class Player_online;


class Player_cloud
{
public:
  //Constructor / Destructor
  Player_cloud(Player_online* online);
  ~Player_cloud();

public:
  //Selection function
  void select_bySubsetID(Cloud* cloud, int ID_subset);
  bool select_rangeLimit(Cloud* cloud, int& ID_subset);
  void select_setVisibility(Cloud* cloud, int& ID_subset);

  //Player functions
  void player_runtime();
  void player_start();
  void player_pause();
  void player_stop();
  void player_save(Cloud* cloud);
  void player_setFrequency(int frequency);
  void player_selectDirSave();

  inline int* get_frequency(){return &player_frequency;}
  inline int* get_player_subset_range(){return &player_subset_range;}
  inline bool* get_player_isrunning(){return &player_isrunning;}
  inline bool* get_player_ispaused(){return &player_ispaused;}
  inline bool* get_with_restart(){return &player_returnToZero;}
  inline string* get_player_saveas(){return &player_saveas;}

private:
  Scene* sceneManager;
  Timer* timerManager;
  Player_online* onlineManager;

  string player_saveas;
  bool player_isrunning;
  bool player_ispaused;
  bool player_returnToZero;
  bool player_flag_1s;
  int player_frequency;
  int player_subset_range;
};

#endif
