#ifndef OFFLINE_H
#define OFFLINE_H

#include "../../../common.h"

class Scene;
class Timer;
class Online;


class Offline
{
public:
  //Constructor / Destructor
  Offline(Online* online);
  ~Offline();

public:
  //Selection function
  void select_bySubsetID(Cloud* cloud, int ID_subset);
  bool select_rangeLimit(Cloud* cloud, int& ID_subset);

  //Player functions
  void player_runtime();
  void player_start();
  void player_pause();
  void player_stop();
  void player_save(Cloud* cloud);
  void player_setFrequency(int frequency);
  void player_selectDirSave();

  inline int* get_frequency(){return &player_frequency;}
  inline bool* get_player_isrunning(){return &player_isrunning;}
  inline bool* get_player_ispaused(){return &player_ispaused;}
  inline bool* get_with_restart(){return &player_returnToZero;}
  inline string* get_player_saveas(){return &player_saveas;}

private:
  Scene* sceneManager;
  Timer* timerManager;
  Online* onlineManager;

  string player_saveas;
  bool player_isrunning;
  bool player_ispaused;
  bool player_returnToZero;
  bool player_flag_1s;
  int player_frequency;
};

#endif