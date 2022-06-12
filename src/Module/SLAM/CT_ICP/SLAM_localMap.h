#ifndef SLAM_LOCALMAP_H
#define SLAM_LOCALMAP_H

#include "../../../common.h"


class SLAM_localMap
{
public:
  //Constructor / Destructor
  SLAM_localMap();
  ~SLAM_localMap();

public:
  void update_configuration();
  void compute_grid_sampling(Subset* subset);
  void add_pointsToSlamMap(Subset* subset);
  void add_pointsToLocalMap(Frame* frame);
  void end_clearTooFarVoxels(Eigen::Vector3d &current_location);
  void end_slamVoxelization(Cloud* cloud, int frame_max);
  void reset_map();

  inline double* get_map_voxel_size(){return &map_voxel_size;}
  inline double* get_slamMap_voxel_size(){return &slamMap_voxel_size;}
  inline double* get_min_root_distance(){return &min_root_distance;}
  inline double* get_max_root_distance(){return &max_root_distance;}
  inline double* get_max_voxel_distance(){return &max_voxel_distance;}
  inline double* get_min_voxel_distance(){return &min_voxel_distance;}
  inline double* get_grid_voxel_size(){return &grid_sampling_voxel_size;}
  inline int* get_map_voxel_capacity(){return &map_voxel_capacity;}
  inline bool* get_slamMap_voxelized(){return &slamMap_voxelized;}
  inline voxelMap* get_map_local(){return map_local;}

private:
  voxelMap* map_local;
  slamMap* map_cloud;

  double map_voxel_size;
  double slamMap_voxel_size;

  double min_root_distance;
  double max_root_distance;
  double max_voxel_distance;
  double min_voxel_distance;
  double grid_sampling_voxel_size;
  int max_total_point;
  int map_voxel_capacity;
  bool slamMap_voxelized;
};


#endif
