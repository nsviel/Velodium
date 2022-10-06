#ifndef SLAM_MAP_H
#define SLAM_MAP_H

#include "../../../common.h"

class SLAM;
class Scene;


class SLAM_map
{
public:
  //Constructor / Destructor
  SLAM_map(SLAM* slam);
  ~SLAM_map();

public:
  //Main function
  void update_configuration();
  void compute_grid_sampling(Subset* subset);
  void update_map(Cloud* cloud, int subset_ID);
  void update_map_parameter(Frame* frame);
  void reset_map_smooth();
  void reset_map_hard();

  //Sub-function
  void add_pointsToLocalMap(Frame* frame);
  void end_clearTooFarVoxels(Eigen::Vector3d &current_location);

  inline slamap* get_local_map(){return local_map;}
  inline double* get_min_root_distance(){return &min_root_distance;}
  inline double* get_max_root_distance(){return &max_root_distance;}
  inline double* get_max_voxel_distance(){return &max_voxel_distance;}
  inline double* get_min_voxel_distance(){return &min_voxel_distance;}
  inline double* get_grid_voxel_size(){return &grid_voxel_width;}
  inline double* get_map_voxel_size(){return &local_map->voxel_width;}
  inline int* get_map_voxel_capacity(){return &local_map->voxel_capacity;}

private:
  Scene* sceneManager;
  slamap* local_map;

  double min_root_distance;
  double max_root_distance;
  double max_voxel_distance;
  double min_voxel_distance;
  double grid_voxel_width;
  int max_total_point;
};


#endif
