#ifndef SLAM_NORMAL_H
#define SLAM_NORMAL_H

#include "../Base/struct_voxelMap.h"
#include "../Base/common.h"

#include <mutex>
#include <shared_mutex>

class SLAM;
class SLAM_map;


class SLAM_normal
{
public:
  //Constructor / Destructor
  SLAM_normal(SLAM* slam);
  ~SLAM_normal();

public:
  //Main function
  void update_configuration();
  void compute_normal(Frame* frame);

  inline void set_nb_thread(int value){this->nb_thread = value;}
  inline double* get_knn_voxel_width(){return &knn_voxel_width;}
  inline int* get_knn_max_nn(){return &knn_max_nn;}
  inline int* get_knn_voxel_search(){return &knn_voxel_search;}

private:
  //Sub-function
  vector<Eigen::Vector3d> compute_kNN_search(Eigen::Vector3d &point);
  void compute_knn_normal(Frame* frame, vector<Eigen::Vector3d>& kNN, int i);
  void compute_normal_direction(Frame* frame, int i);

private:
  SLAM_map* slam_map;

  double knn_voxel_width;
  int knn_voxel_search;
  int knn_max_nn;
  int nb_thread;
  mutex Mutex;
};


#endif
