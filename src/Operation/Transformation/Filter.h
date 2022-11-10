#ifndef FILTER_H
#define FILTER_H

class Node_operation;
class Scene;
class Attribut;
class Configuration;

#include "../../common.h"


class Filter
{
public:
  //Constructor / Destructor
  Filter(Node_operation* node_ope);
  ~Filter();

public:
  void update_configuration();

  void filter_maxAngle(Cloud* cloud, float sampleAngle);
  void filter_sphere();
  void filter_sphere_cloud(Cloud* cloud);
  void filter_sphere_subset(Subset* subset);
  void filter_cylinder_cloud(Cloud* cloud);
  void filter_cylinder_subset(Subset* subset);

  //Setters / Getters
  inline void set_sphereDiameter(float value){this->sphere_D = value;}
  inline float* get_cyl_r_min(){return &cyl_r_min;}
  inline float* get_cyl_r_max(){return &cyl_r_max;}
  inline float* get_cyl_z_min(){return &cyl_z_min;}
  inline float* get_sphere_min(){return &sphere_min;}
  inline float* get_sphere_max(){return &sphere_max;}

private:
  Configuration* configManager;
  Scene* sceneManager;
  Attribut* attribManager;

  float sphere_D;
  float sphere_min;
  float sphere_max;
  float cyl_r_min;
  float cyl_r_max;
  float cyl_z_min;
  bool verbose;
};

#endif
