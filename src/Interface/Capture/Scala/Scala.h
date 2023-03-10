#ifndef SCALA_H
#define SCALA_H

// Server side implementation of UDP client-server model

#include "../../../Scene/Base/struct_data_file.h"
#include "../../../common.h"

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
  void start_watcher();
  void stop_watcher();

  void lidar_create_subset(Data_file* udp_packet);

  inline Cloud* get_obj_capture(){return subset_capture;}
  inline bool* get_is_newSubset(){return &is_new_subset;}
  inline bool* get_is_scala_capturing(){return &is_capturing;}
  inline bool* get_run_capture(){return &run_capture;}
  inline int* get_capture_port(){return &capture_port;}

private:
  Extractor* extractManager;

  Cloud* subset_capture;
  string path_scala;
  string path_scala_file;
  bool flag_newScala;
  bool is_capturing;
  bool run_capture;
  bool is_new_subset;
  int capture_port;
  std::thread thread_scala;
};

#endif
