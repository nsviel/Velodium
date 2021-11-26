#ifndef SLAM_NORMAL_H
#define SLAM_NORMAL_H

#include "../../../common.h"

#include <queue>
#include <map>

using iNN = std::tuple<double, Eigen::Vector3d>;
struct Comparator {
  bool operator()(const iNN &left, const iNN &right) const {return std::get<0>(left) < std::get<0>(right);}
};
using priority_queue_iNN = std::priority_queue<iNN, std::vector<iNN>, Comparator>;
typedef std::map<string, vector<Eigen::Vector3d>> voxelMap;


class SLAM_normal
{
public:
  //Constructor / Destructor
  SLAM_normal();
  ~SLAM_normal();

public:
  //Main function
  void compute_frameNormal(Frame* frame, voxelMap& map);

private:
  //Sub functions
  vector<Eigen::Vector3d> compute_kNN_search(Eigen::Vector3d &point, voxelMap map);
  void compute_normal(vector<Eigen::Vector3d>& kNN, int i);
  void compute_normals_reorientToOrigin(Frame* frame);

private:
  vector<Eigen::Vector3d> Nxyz;
  vector<Eigen::Vector3d> NN;
  vector<float> a2D;

  float size_voxelMap;
};


#endif
