#ifndef FRAME_STRUCT_H
#define FRAME_STRUCT_H

#include <vector>
#include <Eigen/Dense>


struct Frame{ //SLAM stuff
  int ID;
  float time_slam;

  std::vector<Eigen::Vector3d> xyz;
  std::vector<Eigen::Vector3d> xyz_raw;
  std::vector<Eigen::Vector3d> Nptp;
  std::vector<Eigen::Vector3d> NN; // Nearest Neighbor
  std::vector<float> a2D;
  std::vector<float> ts_n;

  //Begin pose
  Eigen::Matrix3d rotat_b;
  Eigen::Vector3d trans_b;

  //End pose
  Eigen::Matrix3d rotat_e;
  Eigen::Vector3d trans_e;

  //Reset all variables
  void reset(){
    xyz.clear();
    xyz_raw.clear();
    Nptp.clear();
    NN.clear();
    a2D.clear();
    ts_n.clear();

    this->rotat_b = Eigen::Matrix3d::Identity();
    this->rotat_e = Eigen::Matrix3d::Identity();
    this->trans_b = Eigen::Vector3d::Zero();
    this->trans_e = Eigen::Vector3d::Zero();
  }
};

#endif
