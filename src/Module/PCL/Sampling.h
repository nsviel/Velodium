#ifdef WITH_PCL
#ifndef SAMPLING_H
#define SAMPLING_H

class Scene;
class Attribut;

#include "../../common.h"

#include <pcl/filters/random_sample.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/octree/octree.h>


class Sampling
{
public:
  //Constructor / Destructor
  Sampling();
  ~Sampling();

public:
  void sampling_gui();

  void sampling_random(Subset* subset);
  void sampling_outlier(Subset* subset);
  void sampling_statistical(Subset* subset);
  void sampling_spaceRadius_PCL(Subset* subset);
  void sampling_spaceRadius(Subset* subset, float resolution);

  //Setters / Getters
  inline void set_sampling(int value){this->sampling_percentagePts = value;}
  inline void set_sampling_squareSize(float value){this->sampling_squareSize = value;}
  inline void set_sampling_radius(float value){this->sampling_radius = value;}
  inline void set_samplingstd(float value){this->sampling_std = value;}

private:
  Scene* sceneManager;
  Attribut* attribManager;

  int sampling_percentagePts;
  float sampling_squareSize;
  float sampling_radius;
  float sampling_std;
};

#endif
#endif
