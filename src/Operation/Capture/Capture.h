#ifndef CAPTURE_H
#define CAPTURE_H

#include "../../common.h"

class Node_operation;
class Scene;
class Loader;
class Extractor;
class Node_module;
class Configuration;
class Node_operation;

class Scala;
class Velodyne;


class Capture
{
public:
  //Constructor / Destructor
  Capture(Node_operation* node_ope);
  ~Capture();

public:
  //Main functions
  void update_configuration();
  void runtime();

  //Start / stop functions
  void start_new_capture(string model);
  void stop_capture();

  //LiDAR specific functions
  void runtime_velodyne();
  void runtime_scala();
  void capture_vlp16();
  void capture_scala();

  //Subfunctions
  void operation_new_subset(Subset* subset);
  void supress_nullpoints(Subset* subset);

  inline Scala* get_scalaManager(){return scalaManager;}
  inline Velodyne* get_veloManager(){return veloManager;}
  inline Cloud* get_cloud_capture(){return cloud_capture;}
  inline bool* get_is_capturing(){return &is_capturing;}
  inline int* get_capture_port(){return &capture_port;}
  inline int get_capture_time(){return capture_time;}
  inline int get_capture_nb_point(){return capture_nb_point;}
  inline int get_capture_nb_point_raw(){return capture_nb_point_raw;}

private:
  Node_module* node_module;
  Scene* sceneManager;
  Loader* loaderManager;
  Configuration* configManager;
  Node_operation* node_ope;

  Cloud* cloud_capture;
  Scala* scalaManager;
  Velodyne* veloManager;

  string lidar_model;
  bool is_capture_finished;
  bool is_capturing;
  bool is_first_run;
  int capture_time;
  int capture_port;
  int capture_nb_point;
  int capture_nb_point_raw;
  int ID_capture;
};

#endif
