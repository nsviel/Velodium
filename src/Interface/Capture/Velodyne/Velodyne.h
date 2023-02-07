#ifndef VELODYNE_H
#define VELODYNE_H

// Server side implementation of UDP client-server model

#include "../../../Engine/Data/struct_data_cap.h"
#include "../../../common.h"

#include <thread>

class Node_interface;
class Extractor;

class Capture_frame;
class Capture_server;
class Parser_VLP16;


class Velodyne
{
public:
  //Constructor / Destructor
  Velodyne(Node_interface* node_interface);
  ~Velodyne();

public:
  //Recording functions
  void start_watcher(int port);
  void stop_watcher();
  Subset* get_subset_capture();

  //LiDAR function
  void lidar_start_motor();
  void lidar_stop_motor();
  void lidar_check_status();
  void lidar_check_connection();

  //LiDAR parametrization
  void lidar_set_rpm(int value);
  void lidar_set_cameraFOV_min(int fov_min);
  void lidar_set_cameraFOV_max(int fov_max);
  void lidar_set_cameraFOV(int min, int max);

  inline bool* get_is_newSubset(){return &is_newSubset;}
  inline bool* get_is_connected(){return &is_connected;}
  inline bool* get_is_rotating(){return &is_rotating;}
  inline bool* get_run_capture(){return &run_capture;}
  inline bool get_is_first_run(){return is_first_run;}
  inline int* get_capture_port(){return &capture_port;}
  inline int get_rot_freq(){return rot_freq;}
  inline int get_rot_rpm(){return rot_rpm;}
  inline int get_fov_min(){return fov_min;}
  inline int get_fov_max(){return fov_max;}
  inline float get_time_frame(){return time_frame;}
  inline float get_time_packet(){return time_packet;}
  inline string* get_lidar_ip(){return &lidar_ip;}

private:
  Extractor* extractManager;
  Capture_frame* frameManager;
  Capture_server* serverManager;
  Parser_VLP16* vlp16Parser;

  Subset* subset_capture;
  Data_cap udp_capture;

  float time_frame;
  float time_packet;
  string lidar_ip;
  int rot_freq, rot_rpm;
  int fov_min, fov_max;
  int capture_port;
  bool is_newSubset;
  bool run_capture;
  bool is_rotating;
  bool is_connected;
  bool is_first_run;
  thread thread_capture;
};

#endif
