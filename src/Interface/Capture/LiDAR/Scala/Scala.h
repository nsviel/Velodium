#ifndef SCALA_H
#define SCALA_H

// Server side implementation of UDP client-server model

#include "../../../../Engine/Data/struct_UDPpacket.h"
#include "../../../../common.h"

#include <thread>

class Node_interface;
class Extractor;


class Scala
{
public:
  //Constructor / Destructor
  Scala(Node_interface* node_interface);
  ~Scala();

public:
  //Recording functions
  void lidar_start_watcher();
  void lidar_create_subset(udpPacket* udp_packet);

  inline Subset* get_subset_capture(){return subset_capture;}
  inline bool* get_is_newSubset(){return &is_new_subset;}
  inline bool* get_is_scala_capturing(){return &is_capturing;}
  inline int* get_capture_port(){return &capture_port;}

private:
  Extractor* extractManager;

  Subset* subset_capture;
  string path_scala;
  string path_scala_file;
  bool flag_newScala;
  bool is_capturing;
  bool is_new_subset;
  int capture_port;
  std::thread thread_scala;
};

#endif
