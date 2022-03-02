#ifndef SCALA_H
#define SCALA_H

// Server side implementation of UDP client-server model

#include "../../../../Engine/Data/struct_UDPpacket.h"
#include "../../../../common.h"

#include <thread>

class Engine_node;
class Extractore;

class UDP_frame;
class UDP_server;
class UDP_parser_Scala;


class Scala
{
public:
  //Constructor / Destructor
  Scala(Engine_node* node_engine);
  ~Scala();

public:
  //Recording functions
  void lidar_start_watcher();
  void lidar_create_subset(udpPacket* udp_packet);

  inline Subset* get_subset_capture(){return subset_capture;}
  inline bool* get_is_newSubset(){return &is_newSubset;}
  inline bool* get_is_scala_capturing(){return &is_capturing;}

private:
  Extractore* extractManager;

  UDP_frame* frameManager;
  UDP_server* udpServManager;
  UDP_parser_Scala* udp_scalaManager;

  Subset* subset_capture;

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
