#ifndef LIGHT_H
#define LIGHT_H

#include "../../Scene/Data/Data.h"
#include "../../common.h"

class Transformation;
class Loader;
class Pose;
class Scene;
class GPU_data;


class Light
{
public:
  //Constructor / Destructor
  Light();
  ~Light();

public:
  void init();

private:
  Loader* loaderManager;
  Transformation* transformManager;
  Pose* poseManager;
  Scene* sceneManager;
  GPU_data* gpuManager;
  Data* data;
};

#endif
