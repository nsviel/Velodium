#ifndef CLOUDPLAYER_H
#define CLOUDPLAYER_H

#include "../../common.h"

class Scene;
class Timer;


class CloudPlayer
{
public:
  //Constructor / Destructor
  CloudPlayer();
  ~CloudPlayer();

public:
  //Main function
  void select_byFrameID(Cloud* cloud, int subset_selected);
  void update_frame_ID(Cloud* cloud);
  void supress_firstSubset(Cloud* cloud);

  //Subfunctions
  void playCloud_start();
  void playCloud_pause();
  void playCloud_stop();
  void playCloud_save(Cloud* cloud);
  void play_setFrequency(int frequency);
  void playCloud_selectDirSave();

  inline int* get_frequency(){return &frequency;}
  inline int* get_frame_ID(){return &subset_selected;}
  inline int* get_frame_max_ID(){return &frame_max_ID;}
  inline int* get_frame_max_nb(){return &frame_max_nb;}
  inline int* get_frame_display_range(){return &frame_display_range;}
  inline float* get_frame_ID_ts(){return &frame_ID_ts;}
  inline bool* get_player_isrunning(){return &player_isrunning;}
  inline bool* get_with_restart(){return &with_restart;}
  inline string* get_saveas(){return &saveas;}

private:
  Scene* sceneManager;
  Timer* timerManager;

  string saveas;
  float frame_ID_ts;
  bool player_isrunning;
  bool with_restart;
  int cpt;
  int frame_display_range;
  int frequency;
  int subset_selected;
  int frame_max_ID, frame_max_nb;
};

#endif
