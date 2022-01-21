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
  void compute_gridSampling(Subset* subset);
  void add_pointsToSlamMap(Subset* subset);
  void add_pointsToLocalMap(Frame* frame);
  void end_clearTooFarVoxels(Eigen::Vector3d &current_location);
  void end_slamVoxelization(Cloud* cloud, int frame_max);
  void reset();

  inline float* get_voxel_size_localMap(){return &voxel_size_localMap;}
  inline float* get_voxel_size_slamMap(){return &voxel_size_slamMap;}
  inline float* get_min_subset_distance(){return &min_subset_distance;}
  inline float* get_max_subset_distance(){return &max_subset_distance;}
  inline float* get_max_voxel_distance(){return &max_voxel_distance;}
  inline float* get_min_voxel_distance(){return &min_voxel_distance;}
  inline float* get_grid_voxel_size(){return &grid_voxel_size;}
  inline int* get_map_max_voxelNbPoints(){return &map_max_voxelNbPoints;}
  inline bool* get_slamMap_voxelized(){return &slamMap_voxelized;}
  inline voxelMap* get_localmap(){return map;}

private:
  voxelMap* map;
  slamMap* map_cloud;

  float voxel_size_localMap;
  float voxel_size_slamMap;

  float min_subset_distance;
  float max_subset_distance;
  float max_voxel_distance;
  float min_voxel_distance;
  float grid_voxel_size;
  int map_max_voxelNbPoints;
  bool slamMap_voxelized;
};


#endif
