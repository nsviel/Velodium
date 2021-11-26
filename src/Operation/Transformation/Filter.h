#ifndef FILTER_H
#define FILTER_H

class Scene;
class Attribut;

#include "../../common.h"

class Filter
{
public:
  //Constructor / Destructor
  Filter();
  ~Filter();

public:
  void filter_maxAngle(Cloud* cloud, float sampleAngle);
  void filter_sphereCleaning();

  //Setters / Getters
  inline void set_sphereDiameter(float value){this->sphereDiameter = value;}

private:
  Scene* sceneManager;
  Attribut* attribManager;

  float sphereDiameter;
};

#endif
