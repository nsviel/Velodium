#ifndef VELODYNE_H
#define VELODYNE_H

// Server side implementation of UDP client-server model

#include "UDP/struct_UDPpacket.h"

#include "../../common.h"
#include "../../Engine/Data/struct_dataFile.h"

#include <thread>

class UDP_frame;
class UDP_server;
class UDP_parser_VLP16;

class Loader;
class dataExtraction;
class Timer;


class Velodyne
{
public:
  //Constructor / Destructor
  Velodyne();
  ~Velodyne();

public:
  //Recording functions
  void run_capture();
  void recording_selectDirSave();
  void onrun_ope();

  //LiDAR functions
  void lidar_start();
  void lidar_stop();
  void lidar_startNewCapture();
  void lidar_get_status();
  bool lidar_get_is_connected();
  void lidar_set_rpm(int value);
  void lidar_set_fov_min(int fov_min);
  void lidar_set_fov_max(int fov_max);

  inline bool* get_is_connected(){return &is_connected;}
  inline bool* get_is_rotating(){return &is_rotating;}
  inline bool* get_is_capturing(){return &is_capturing;}
  inline bool* get_is_recording(){return &is_recording;}
  inline bool* get_is_record_t_frame(){return &is_record_t_frame;}
  inline bool* get_is_record_n_frame(){return &is_record_n_frame;}
  inline float* get_record_t_frame_max(){return &is_record_t_frame_max;}
  inline int* get_record_n_frame_max(){return &record_n_frame_max;}
  inline int get_rot_freq(){return rot_freq;}
  inline int get_rot_rpm(){return rot_rpm;}
  inline int get_fov_min(){return fov_min;}
  inline int get_fov_max(){return fov_max;}
  inline string* get_saveas(){return &saveas;}

private:
  Loader* loaderManager;
  UDP_frame* frameManager;
  UDP_server* udpServManager;
  UDP_parser_VLP16* udpParsManager;
  dataExtraction* extractManager;
  Timer* timerManager;
  udpPacket* frame;

  int rot_freq;
  int rot_rpm;
  int fov_min, fov_max;
  int subset_selected;
  int record_n_frame_nb;
  int record_n_frame_max;
  int record_t_frame_sec;
  float is_record_t_frame_max;
  bool atleastoneframe;
  bool has_started;
  bool is_capturing;
  bool is_rotating;
  bool is_connected;
  bool is_recording;
  bool is_record_n_frame;
  bool is_record_t_frame;
  string saveas;
  std::thread m_thread;

  Cloud* cloud;
  bool is_first_run;

};

#endif
