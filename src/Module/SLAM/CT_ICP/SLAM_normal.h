#ifndef SLAM_NORMAL_H
#define SLAM_NORMAL_H

#include "../../../Engine/Data/struct_voxelMap.h"

#include "../../../common.h"

#include <mutex>
#include <shared_mutex>


class SLAM_normal
{
public:
  //Constructor / Destructor
  SLAM_normal();
  ~SLAM_normal();

public:
  //Main function
  void update_configuration();
  void compute_normal(Frame* frame, voxelMap* map);

  inline void set_nb_thread(int value){this->nb_thread = value;}
  inline double* get_knn_voxel_capacity(){return &knn_voxel_capacity;}
  inline int* get_knn_max_nn(){return &knn_max_nn;}
  inline int* get_knn_voxel_search(){return &knn_voxel_search;}

private:
  //Sub functions
  vector<Eigen::Vector3d> compute_kNN_search(Eigen::Vector3d &point, voxelMap* map);
  void compute_knn_normal(Frame* frame, vector<Eigen::Vector3d>& kNN, int i);
  void compute_normal_direction(Frame* frame, int i);

private:
  double knn_voxel_capacity;
  int knn_max_nn;
  int knn_voxel_search;
  int nb_thread;
  mutex Mutex;
};


#endif
