#ifndef FITTING_H
#define FITTING_H

class Node_operation;
class Glyphs;

#include "../../common.h"

#include <Eigen/Core>
#include <Eigen/Dense>


class Fitting
{
public:
  Fitting();
  ~Fitting();

public:
  //Sphere fitting
  void Sphere_cloudToCenter_all(list<Cloud*>* list_cloud);
  void Sphere_cloudToCenter(Subset* subset);
  vec3 Sphere_FindCenter(Subset* subset);

  //Plane fitting
  std::pair<vec3, vec3> plane_fitting_normal(vector<vec3>& vec);

private:
  float Radius;
};
#endif
