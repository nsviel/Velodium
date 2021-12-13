#ifndef SLAM_NORMAL_H
#define SLAM_NORMAL_H

#include "../struct_voxelMap.h"

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
  int nb_thread;
  mutable std::shared_mutex mutex_;
};


#endif