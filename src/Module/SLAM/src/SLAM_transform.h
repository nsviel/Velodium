#ifndef SLAM_TRANSFORM_H
#define SLAM_TRANSFORM_H

#include "../../../common.h"

class Scene;
class SLAM;
class Object;
class SLAM_map;
class SLAM_sampling;


class SLAM_transform
{
public:
  //Constructor / Destructor
  SLAM_transform(SLAM* slam);
  ~SLAM_transform();

public:
  //Main function
  void compute_preprocessing(Cloud* cloud, int subset_ID);

  //Tranformation function
  void transform_frame(Frame* frame);
  void transform_subset(Subset* subset);
  void distort_frame(Frame* frame);

private:
  Scene* sceneManager;
  Object* objectManager;
  SLAM_map* slam_map;
  SLAM_sampling* slam_sampling;

  bool with_distorsion;
};


#endif
