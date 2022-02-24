#ifndef FITTING_H
#define FITTING_H

class Operation_node;
class Scene;
class Glyphs;

#include "../../common.h"


class Fitting
{
public:
  Fitting(Operation_node* node_ope);
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

  float Radius;
};
#endif
