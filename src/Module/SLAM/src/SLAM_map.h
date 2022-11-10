#ifndef SLAM_MAP_H
#define SLAM_MAP_H

#include "../../../common.h"

class SLAM;
class Scene;
class Pather;


class SLAM_map
{
public:
  //Constructor / Destructor
  SLAM_map(SLAM* slam);
  ~SLAM_map();

public:
  //Main function
  void update_configuration();
  void update_map(Cloud* cloud, int subset_ID);
  void reset_map();

  //Sub-function
  void add_pointToMap(slamap* map, Frame* frame);
  void add_pointToMap(slamap* map, Subset* subset);
  void end_clearTooFarVoxels(slamap* map, Eigen::Vector3d &pose);
  void save_local_cloud();

  inline slamap* get_local_map(){return local_map;}
  inline slamap* get_local_cloud(){return local_cloud;}
  inline bool* get_with_local_cloud(){return &with_local_cloud;}
  inline double* get_max_voxel_distance(){return &max_voxel_distance;}
  inline double* get_min_dist_point_in_voxel(){return &min_dist_point_in_voxel;}
  inline double* get_map_voxel_size(){return &local_map->voxel_width;}
  inline int* get_map_voxel_capacity(){return &local_map->voxel_capacity;}

private:
  Scene* sceneManager;
  Pather* patherManager;

  slamap* local_map;
  slamap* local_cloud;

  bool with_local_cloud;

  double max_voxel_distance;
  double min_dist_point_in_voxel;
};


#endif
