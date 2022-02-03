#ifndef CAPTURE_H
#define CAPTURE_H

#include "../../common.h"

class Scene;
class Loader;
class dataExtraction;

class Scala;
class Velodyne;


class Capture
{
public:
  //Constructor / Destructor
  Capture();
  ~Capture();

public:
  //Recording functions
  void init_new_capture();
  void runtime_check_newSubset();

  inline Scala* get_scalaManager(){return scalaManager;}
  inline Velodyne* get_veloManager(){return veloManager;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  dataExtraction* extractManager;

  Cloud* cloud_capture;
  Scala* scalaManager;
  Velodyne* veloManager;

  bool with_justOneFrame;
  int ID_capture;
};

#endif
