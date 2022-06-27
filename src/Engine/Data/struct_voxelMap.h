#ifndef VOXELMAP_STRUCT_H
#define VOXELMAP_STRUCT_H

#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <tsl/robin_map.h>
#include <queue>


//iNN search
using iNN = std::tuple<double, Eigen::Vector3d>;
struct Comparator {bool operator()(const iNN &left, const iNN &right) const {return std::get<0>(left) < std::get<0>(right);}};
using priority_queue_iNN = std::priority_queue<iNN, std::vector<iNN>, Comparator>;

typedef tsl::robin_map<int, std::vector<Eigen::Vector3d>> voxelMap;
typedef tsl::robin_map<int, std::vector<Eigen::Vector3d>>::iterator voxelMap_it;
typedef tsl::robin_map<int, std::vector<Eigen::Vector4d>> gridMap;
typedef tsl::robin_map<int, std::vector<glm::vec3>> slamMap;

struct slamap{
  //---------------------------

  voxelMap map;

  double voxel_width;
  int voxel_capacity;
  int linked_cloud_ID;
  int linked_subset_ID;
  int current_frame_ID;
  int size;

  int get_signature(int kx, int ky, int kz){
    int key = (kx*2000 + ky)*1000 + kz;
    return key;
  }

  //---------------------------
};


#endif
