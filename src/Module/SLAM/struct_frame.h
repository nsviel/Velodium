#ifndef FRAME_STRUCT_H
#define FRAME_STRUCT_H

#include <vector>
#include <Eigen/Dense>


struct Frame{ //SLAM stuff
  int ID;

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

  //Stats
  float time_slam;
  bool is_slamed;
  int map_size_abs;
  int map_size_rlt;
  glm::vec3 trans_abs;
  glm::vec3 rotat_abs;
  glm::vec3 trans_rlt;
  glm::vec3 rotat_rlt;

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

    this->time_slam = 0;
    this->map_size_abs = 0;
    this->map_size_rlt = 0;
    this->is_slamed = false;

    this->trans_abs = glm::vec3(0, 0, 0);
    this->rotat_abs = glm::vec3(0, 0, 0);
    this->trans_rlt = glm::vec3(0, 0, 0);
    this->rotat_rlt = glm::vec3(0, 0, 0);
  }
};

#endif
