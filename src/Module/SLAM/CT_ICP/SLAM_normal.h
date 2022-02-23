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
  void compute_frameNormal(Frame* frame, voxelMap* map);

  inline void set_nb_thread(int value){this->nb_thread = value;}
  inline float* get_size_voxelMap(){return &size_voxelMap;}
  inline int* get_max_number_neighbors(){return &max_number_neighbors;}
  inline int* get_voxel_searchSize(){return &voxel_searchSize;}

private:
  //Sub functions
  vector<Eigen::Vector3d> compute_kNN_search(Eigen::Vector3d &point, voxelMap* map);
  void compute_normal(vector<Eigen::Vector3d>& kNN, int i);
  void compute_normals_reorientToOrigin(Frame* frame);

private:
  vector<Eigen::Vector3d> Nxyz;
  vector<Eigen::Vector3d> NN;
  vector<float> a2D;

  float size_voxelMap;
  int max_number_neighbors;
  int voxel_searchSize;
  int nb_thread;
  mutex Mutex;
};


#endif
