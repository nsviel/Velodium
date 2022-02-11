#ifndef CAPTURE_H
#define CAPTURE_H

#include "../../common.h"

class Scene;
class Loader;
class dataExtraction;
class Module_node;
class Player_online;

class Scala;
class Velodyne;


class Capture
{
public:
  //Constructor / Destructor
  Capture(Module_node* node_module);
  ~Capture();

public:
  //Recording functions
  void start_new_capture();
  void stop_capture();
  void runtime_capturing();
  void supress_nullpoints(Subset* subset);
  void remove_subset_last(Cloud* cloud);

  inline Scala* get_scalaManager(){return scalaManager;}
  inline Velodyne* get_veloManager(){return veloManager;}
  inline int* get_nb_subset_max(){return &nb_subset_max;}
  inline bool get_is_capturing(){return is_capturing;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Player_online* onlineManager;

  Cloud* cloud_capture;
  Scala* scalaManager;
  Velodyne* veloManager;

  bool with_justOneFrame;
  bool is_capturing;
  int nb_subset_max;
  int ID_capture;
  int ID_subset;
};

#endif
