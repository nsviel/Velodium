#ifndef Fitting_H
#define Fitting_H

class Scene;
class Glyphs;
class Attribut;

#include "../../common.h"

class Fitting
{
public:
  Fitting();
  ~Fitting();

public:
  //Sphere fitting
  void Sphere_cloudToCenter_all();
  void Sphere_cloudToCenter(Subset* subset);
  vec3 Sphere_FindCenter(Subset* subset);

  //Plane fitting
  void Plane_cloud_all();

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  Attribut* attribManager;

  float Radius;
};
#endif
