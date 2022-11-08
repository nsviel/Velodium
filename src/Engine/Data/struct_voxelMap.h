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
typedef tsl::robin_map<int, std::vector<Eigen::Vector4d>> cloudMap;
typedef tsl::robin_map<int, std::vector<Eigen::Vector4d>>::iterator cloudMap_it;

struct slamap{
  //---------------------------

  voxelMap map;
  cloudMap cloud;

  //Parameter
  double voxel_width;
  int voxel_capacity;
  int size;

  //IDs
  int linked_cloud_ID;
  int linked_subset_ID;
  int current_frame_ID;

  //Pose
  Eigen::Matrix3d rotat_b;
  Eigen::Vector3d trans_b;
  Eigen::Matrix3d rotat_e;
  Eigen::Vector3d trans_e;

  //Function
  int get_signature(int kx, int ky, int kz){
    int key = (kx*2000 + ky)*1000 + kz;
    return key;
  }

  void reset(){
    this->map.clear();
    this->cloud.clear();

    this->linked_cloud_ID = -1;
    this->linked_subset_ID = -1;
    this->current_frame_ID = 0;

    this->rotat_b = Eigen::Matrix3d::Identity();
    this->rotat_e = Eigen::Matrix3d::Identity();
    this->trans_b = Eigen::Vector3d::Zero();
    this->trans_e = Eigen::Vector3d::Zero();
  }

  //---------------------------
};


#endif
