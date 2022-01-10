#ifndef CLOUDPLAYER_H
#define CLOUDPLAYER_H

#include "../../common.h"

class Camera;
class Scene;
class Heatmap;
class Timer;
class CT_ICP;


class CloudPlayer
{
public:
  //Constructor / Destructor
  CloudPlayer(Camera* camManager);
  ~CloudPlayer();

public:
  //Main function
  void select_byFrameID(Cloud* cloud, int subset_selected);
  void update_frame_ID(Cloud* cloud);
  void supress_firstSubset(Cloud* cloud);
  void camera_followUp(Subset* subset);

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
  inline bool* get_all_frame_visible(){return &all_frame_visible;}
  inline bool* get_playCloud_isrunning(){return &playCloud_isrunning;}
  inline bool* get_camera_follow(){return &camera_follow;}
  inline bool* get_with_slam(){return &with_slam;}
  inline bool* get_with_restart(){return &with_restart;}
  inline string* get_saveas(){return &saveas;}

private:
  Scene* sceneManager;
  Heatmap* heatmapManager;
  Camera* cameraManager;
  Timer* timerManager;
  CT_ICP* slamManager;

  int cpt;
  int frame_display_range;
  int frequency;
  int subset_selected;
  int frame_max_ID, frame_max_nb;
  float frame_ID_ts;
  bool all_frame_visible;
  bool playCloud_isrunning;
  bool with_restart;
  bool camera_follow;
  bool with_slam;
  string saveas;
  vec3 camera_moved;
};

#endif
