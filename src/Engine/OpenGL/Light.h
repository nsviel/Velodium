#ifndef LIGHT_H
#define LIGHT_H

#include "../../common.h"

class Node_engine;
class Transformation;
class Loader;
class Pose;
class Scene;


class Light
{
public:
  //Constructor / Destructor
  Light(Node_engine* node);
  ~Light();

public:
  void init();

private:
  Loader* loaderManager;
  Transformation* transformManager;
  Pose* poseManager;
  Scene* sceneManager;
};

#endif
