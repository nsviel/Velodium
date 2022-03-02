#ifndef CAPTURE_H
#define CAPTURE_H

#include "../../../common.h"

class Scene;
class Loader;
class Extractore;
class Module_node;

class Scala;
class Velodyne;


class Capture
{
public:
  //Constructor / Destructor
  Capture(Module_node* node_module);
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

  inline int* get_nb_subset_max(){return &nb_subset_max;}
  inline bool get_is_capture_watcher(){return is_capturing;}

private:
  Scene* sceneManager;
  Loader* loaderManager;

  Cloud* cloud_capture;
  Scala* scalaManager;
  Velodyne* veloManager;

  string lidar_capture;
  bool with_justOneFrame;
  bool is_capturing;
  int nb_subset_max;
  int ID_capture;
};

#endif
