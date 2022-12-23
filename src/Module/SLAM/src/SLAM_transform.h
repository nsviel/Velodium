#ifndef SLAM_TRANSFORM_H
#define SLAM_TRANSFORM_H

#include "../../../common.h"

class Scene;
class SLAM;
class Object;
class SLAM_map;


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
  void transform_glyph(Subset* subset);

  //Sampling functions
  void sub_sampling_subset(Subset* subset);
  void grid_sampling_subset(Subset* subset);

  //Specific functions
  void distort_frame(Frame* frame);
  void reset_glyph();

  inline double* get_min_root_distance(){return &min_root_distance;}
  inline double* get_max_root_distance(){return &max_root_distance;}
  inline double* get_grid_voxel_size(){return &grid_voxel_width;}
  inline int* get_max_keypoint(){return &max_keypoint;}

private:
  Scene* sceneManager;
  Object* objectManager;
  SLAM_map* slam_map;

  double min_root_distance;
  double max_root_distance;
  double grid_voxel_width;
  bool with_distorsion;
  int max_keypoint;
};


#endif
