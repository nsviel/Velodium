#ifndef VOXELMAP_STRUCT_H
#define VOXELMAP_STRUCT_H

#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <tsl/robin_map.h>
#include <queue>
#include <iostream>


//iNN search
using iNN = std::tuple<double, Eigen::Vector3d>;
struct Comparator {bool operator()(const iNN &left, const iNN &right) const {return std::get<0>(left) < std::get<0>(right);}};
using priority_queue_iNN = std::priority_queue<iNN, std::vector<iNN>, Comparator>;

typedef tsl::robin_map<int, std::vector<Eigen::Vector3d>> voxelMap;
typedef tsl::robin_map<int, std::vector<Eigen::Vector3d>>::iterator voxelMap_it;
typedef tsl::robin_map<int, std::vector<Eigen::Vector4d>> cloudMap;
typedef tsl::robin_map<int, std::vector<Eigen::Vector4d>>::iterator cloudMap_it;

struct slamap{
  //---------------------------

  voxelMap map;
  cloudMap cloud;

  //Parameter
  double voxel_width;
  double voxel_min_point_dist;
  double voxel_max_dist;
  int voxel_capacity;
  int size;

  //Cloud map
  double dist_min;
  double dist_max;

  //IDs
  int linked_col_ID;
  int linked_subset_ID;
  int current_frame_ID;

  //Functions
  void reset();
  int get_signature(int kx, int ky, int kz);

  //---------------------------
};


#endif
