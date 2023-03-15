#ifndef LIGHT_H
#define LIGHT_H

#include "../../Scene/Data/Data.h"
#include "../../common.h"

class Node_engine;
class Transformation;
class Loader;
class Pose;
class Scene;
class GPU_data;
class Shader;


class Light
{
public:
  //Constructor / Destructor
  Light(Node_engine* node);
  ~Light();

public:
  void init();
  void light_being_displaced();

private:
  Shader* shaderManager;
  Loader* loaderManager;
  Transformation* transformManager;
  Pose* poseManager;
  Scene* sceneManager;
  GPU_data* gpuManager;
  Data* data;
};

#endif
