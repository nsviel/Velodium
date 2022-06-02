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
  void end_clearTooFarVoxels(Eigen::Vector3f &current_location);
  void end_slamVoxelization(Cloud* cloud, int frame_max);
  void reset_map();

  inline float* get_map_voxel_size(){return &map_voxel_size;}
  inline float* get_slamMap_voxel_size(){return &slamMap_voxel_size;}
  inline float* get_min_root_distance(){return &min_root_distance;}
  inline float* get_max_root_distance(){return &max_root_distance;}
  inline float* get_max_voxel_distance(){return &max_voxel_distance;}
  inline float* get_min_voxel_distance(){return &min_voxel_distance;}
  inline float* get_grid_voxel_size(){return &grid_sampling_voxel_size;}
  inline int* get_map_max_voxelNbPoints(){return &map_max_voxelNbPoints;}
  inline bool* get_slamMap_voxelized(){return &slamMap_voxelized;}
  inline voxelMap* get_map_local(){return map_local;}

private:
  voxelMap* map_local;
  slamMap* map_cloud;

  float map_voxel_size;
  float slamMap_voxel_size;

  float min_root_distance;
  float max_root_distance;
  float max_voxel_distance;
  float min_voxel_distance;
  float grid_sampling_voxel_size;
  int max_total_point;
  int map_max_voxelNbPoints;
  bool slamMap_voxelized;
};


#endif
