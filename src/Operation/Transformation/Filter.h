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
  void filter_sphereCleaning();
  void filter_subset_cylinder(Subset* subset);
  void filter_cloud_cylinder(Cloud* cloud);

  //Setters / Getters
  inline void set_sphereDiameter(float value){this->sphereDiameter = value;}
  inline float* get_cyl_r_min(){return &cyl_r_min;}
  inline float* get_cyl_r_max(){return &cyl_r_max;}
  inline float* get_cyl_z_min(){return &cyl_z_min;}

private:
  Configuration* configManager;
  Scene* sceneManager;
  Attribut* attribManager;

  float sphereDiameter;
  float cyl_r_min;
  float cyl_r_max;
  float cyl_z_min;
  bool verbose;
};

#endif
