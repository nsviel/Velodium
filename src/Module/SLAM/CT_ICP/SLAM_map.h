#ifndef SLAM_map_H
#define SLAM_map_H

#include "../../../common.h"


class SLAM_map
{
public:
  //Constructor / Destructor
  SLAM_map();
  ~SLAM_map();

public:
  void update_configuration();
  void update_map(Frame* frame);
  void reset_map_hard();
  void reset_map_smooth();

  void compute_grid_sampling(Subset* subset);
  void add_pointsToLocalMap(Frame* frame);
  void end_clearTooFarVoxels(Eigen::Vector3d &current_location);

  inline slamap* get_slam_map(){return slam_map;}
  inline double* get_min_root_distance(){return &min_root_distance;}
  inline double* get_max_root_distance(){return &max_root_distance;}
  inline double* get_max_voxel_distance(){return &max_voxel_distance;}
  inline double* get_min_voxel_distance(){return &min_voxel_distance;}
  inline double* get_grid_voxel_size(){return &grid_voxel_width;}
  inline double* get_map_voxel_size(){return &slam_map->voxel_width;}
  inline int* get_map_voxel_capacity(){return &slam_map->voxel_capacity;}

private:
  slamap* slam_map;

  double min_root_distance;
  double max_root_distance;
  double max_voxel_distance;
  double min_voxel_distance;
  double grid_voxel_width;
  int max_total_point;
};


#endif
