#ifndef CAPTURE_H
#define CAPTURE_H

#include "../../../common.h"

class Interface_node;
class Scene;
class Loader;
class Extractor;
class Module_node;
class Configuration;

class Scala;
class Velodyne;


class Capture
{
public:
  //Constructor / Destructor
  Capture(Interface_node* node_module);
  ~Capture();

public:
  //Main functions
  void runtime_capturing();
  void update_configuration();

  //Start / stop functions
  void start_new_capture(string model);
  void stop_capture();

  //LiDAR specific functions
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
  Module_node* node_module;
  Scene* sceneManager;
  Loader* loaderManager;
  Configuration* configManager;

  Cloud* cloud_capture;
  Scala* scalaManager;
  Velodyne* veloManager;

  string lidar_model;
  bool is_capture_finished;
  bool is_capturing;
  int capture_time;
  int capture_port;
  int capture_nb_point;
  int capture_nb_point_raw;
  int ID_capture;
};

#endif
