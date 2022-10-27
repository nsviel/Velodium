#ifndef FITTING_H
#define FITTING_H

class Node_operation;
class Scene;
class Glyphs;

#include "../../common.h"

#include <Eigen/Core>
#include <Eigen/Dense>


class Fitting
{
public:
  Fitting(Node_operation* node_ope);
  ~Fitting();

public:
  //Sphere fitting
  void Sphere_cloudToCenter_all();
  void Sphere_cloudToCenter(Subset* subset);
  vec3 Sphere_FindCenter(Subset* subset);

  //Plane fitting
  vec3 plane_fitting_normal(vector<vec3>& vec);

private:
  Scene* sceneManager;
  Glyphs* glyphManager;

  float Radius;
};
#endif
