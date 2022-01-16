#ifndef VELODYNE_H
#define VELODYNE_H

// Server side implementation of UDP client-server model

#include "UDP/struct_UDPpacket.h"

#include "../../../common.h"

#include <thread>

class UDP_frame;
class UDP_server;
class UDP_parser_VLP16;

class Scene;
class Timer;
class Capture;


class Velodyne
{
public:
  //Constructor / Destructor
  Velodyne();
  ~Velodyne();

public:
  //Recording functions
  void run_capture();

  //LiDAR functions
  void lidar_start();
  void lidar_stop();
  void lidar_startNewCapture();
  void lidar_get_status();
  bool lidar_get_is_connected();
  void lidar_set_rpm(int value);
  void lidar_set_cameraFOV_min(int fov_min);
  void lidar_set_cameraFOV_max(int fov_max);
  void lidar_set_cameraFOV(int min, int max);

  inline Capture* get_captureManager(){return captureManager;}
  inline bool* get_is_connected(){return &is_connected;}
  inline bool* get_is_rotating(){return &is_rotating;}
  inline bool* get_is_capturing(){return &is_capturing;}
  inline bool* get_is_recording(){return &is_recording;}
  inline int get_rot_freq(){return rot_freq;}
  inline int get_rot_rpm(){return rot_rpm;}
  inline int get_fov_min(){return fov_min;}
  inline int get_fov_max(){return fov_max;}

private:
  Scene* sceneManager;
  Timer* timerManager;
  Capture* captureManager;

  UDP_frame* frameManager;
  UDP_server* udpServManager;
  UDP_parser_VLP16* udpParsManager;

  int time_of_capture;
  int rot_freq, rot_rpm;
  int fov_min, fov_max;
  bool has_started;
  bool is_capturing;
  bool is_rotating;
  bool is_connected;
  bool is_recording;
  bool is_first_run;
  std::thread m_thread;
};

#endif
