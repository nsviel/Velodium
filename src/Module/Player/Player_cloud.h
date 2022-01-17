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
  //Main function
  void select_byFrameID(Cloud* cloud, int subset_selected);
  void update_frame_ID(Cloud* cloud);
  void supress_firstSubset(Cloud* cloud);

  //Subfunctions
  void player_start();
  void player_pause();
  void player_stop();
  void player_save(Cloud* cloud);
  void player_setFrequency(int frequency);
  void player_selectDirSave();

  inline int* get_frequency(){return &player_frequency;}
  inline int* get_frame_ID(){return &subset_selected;}
  inline int* get_frame_max_ID(){return &frame_max_ID;}
  inline int* get_frame_max_nb(){return &frame_max_nb;}
  inline int* get_player_frame_range(){return &frame_display_range;}
  inline float* get_frame_ID_ts(){return &frame_ID_ts;}
  inline bool* get_player_isrunning(){return &player_isrunning;}
  inline bool* get_with_restart(){return &player_returnToZero;}
  inline string* get_saveas(){return &saveas;}

private:
  Scene* sceneManager;
  Timer* timerManager;
  Player_online* onlineManager;

  string saveas;
  int subset_selected;

  float frame_ID_ts;
  int frame_max_ID;
  int frame_max_nb;
  int frame_display_range;

  bool player_isrunning;
  bool player_returnToZero;
  int player_frequency;
};

#endif
