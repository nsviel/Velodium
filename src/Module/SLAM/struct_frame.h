#ifndef FRAME_STRUCT_H
#define FRAME_STRUCT_H

#include <vector>
#include <Eigen/Dense>


struct Frame{ //SLAM stuff
  int ID;

  std::vector<Eigen::Vector3d> xyz;
  std::vector<Eigen::Vector3d> xyz_raw;
  std::vector<Eigen::Vector3d> Nxyz;
  std::vector<Eigen::Vector3d> NN; // Nearest Neighbor
  std::vector<float> a2D;

  std::vector<double> ts_n;
  float ts_b;
  float ts_e;

  //Begin pose
  Eigen::Matrix3d rotat_b;
  Eigen::Vector3d trans_b;

  //End pose
  Eigen::Matrix3d rotat_e;
  Eigen::Vector3d trans_e;
};

#endif
