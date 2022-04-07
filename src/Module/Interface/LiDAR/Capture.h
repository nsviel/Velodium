#ifndef CAPTURE_H
#define CAPTURE_H

#include "../../../common.h"

class Interface_node;
class Scene;
class Loader;
class Extractor;
class Module_node;

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
  void start_new_capture();
  void stop_capture();
  void runtime_capturing();

  //LiDAR specific functions
  void capture_vlp16();
  void capture_scala();

  //Subfunctions
  void operation_new_subset(Subset* subset);
  void supress_nullpoints(Subset* subset);

  inline Scala* get_scalaManager(){return scalaManager;}
  inline Velodyne* get_veloManager(){return veloManager;}
  inline Cloud* get_cloud_capture(){return cloud_capture;}

  inline bool get_is_capture_watcher(){return is_capturing;}
  inline bool* get_is_new_capture(){return &is_new_capture;}

private:
  Module_node* node_module;
  Scene* sceneManager;
  Loader* loaderManager;

  Cloud* cloud_capture;
  Scala* scalaManager;
  Velodyne* veloManager;

  string lidar_model;
  bool is_capturing;
  bool is_new_capture;
  int ID_capture;
};

#endif
