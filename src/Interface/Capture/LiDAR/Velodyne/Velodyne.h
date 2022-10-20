#ifndef VELODYNE_H
#define VELODYNE_H

// Server side implementation of UDP client-server model

#include "../../../../Engine/Data/struct_UDPpacket.h"
#include "../../../../common.h"

#include <thread>

class Node_interface;
class Extractor;

class UDP_frame;
class UDP_server;
class UDP_parser_VLP16;


class Velodyne
{
public:
  //Constructor / Destructor
  Velodyne(Node_interface* node_interface);
  ~Velodyne();

public:
  //Recording functions
  void lidar_start_watcher(int capture_port);
  void lidar_stop_watcher();
  Subset* get_subset_capture();

  //LiDAR motor
  void lidar_start_motor();
  void lidar_stop_motor();

  //LiDAR status
  void lidar_check_status();
  void lidar_check_connection();

  //LiDAR parametrization
  void lidar_set_rpm(int value);
  void lidar_set_cameraFOV_min(int fov_min);
  void lidar_set_cameraFOV_max(int fov_max);
  void lidar_set_cameraFOV(int min, int max);

  //inline Subset* get_subset_capture(){return subset_capture;}
  inline bool* get_is_newSubset(){return &is_newSubset;}
  inline bool* get_is_connected(){return &is_connected;}
  inline bool* get_is_rotating(){return &is_rotating;}
  inline bool* get_is_capturing(){return &is_capturing;}
  inline bool get_is_first_run(){return is_first_run;}
  inline int get_rot_freq(){return rot_freq;}
  inline int get_rot_rpm(){return rot_rpm;}
  inline int get_fov_min(){return fov_min;}
  inline int get_fov_max(){return fov_max;}

private:
  Extractor* extractManager;

  UDP_frame* frameManager;
  UDP_server* udpServManager;
  UDP_parser_VLP16* udp_vlp16Manager;

  Subset* subset_capture;
  udpPacket udp_capture;

  int rot_freq, rot_rpm;
  int fov_min, fov_max;
  bool is_newSubset;
  bool is_capturing;
  bool is_rotating;
  bool is_connected;
  bool is_first_run;
  std::thread thread_capture;
};

#endif
