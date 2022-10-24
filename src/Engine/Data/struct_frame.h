#ifndef FRAME_STRUCT_H
#define FRAME_STRUCT_H

#include <vector>
#include <Eigen/Dense>


struct Frame{ //SLAM stuff
  int ID;

  std::vector<Eigen::Vector3d> xyz;
  std::vector<Eigen::Vector3d> xyz_raw;
  std::vector<Eigen::Vector3d> N_nn; // Normal given by nearest neighbor
  std::vector<Eigen::Vector3d> nn; // Closest point
  std::vector<double> a2D;
  std::vector<double> ts_n;

  Eigen::Vector3d root;

  //Begin pose
  Eigen::Matrix3d rotat_b;
  Eigen::Vector3d trans_b;

  //End pose
  Eigen::Matrix3d rotat_e;
  Eigen::Vector3d trans_e;

  //Assessment stats
  float ego_trans;
  float ego_rotat;
  float diff_trans;
  float diff_rotat;
  float opti_score;

  //Processing time
  float time_save_image;
  float time_save_frame;
  float time_slam;

  //Informative stats
  bool is_slamed;
  int nb_residual;
  int map_size_abs;
  int map_size_rlt;
  glm::vec3 trans_b_rlt;
  glm::vec3 rotat_b_rlt;
  glm::vec3 trans_e_rlt;
  glm::vec3 rotat_e_rlt;
  glm::vec3 angle_b;
  glm::vec3 angle_e;

  //Reset all variables
  void reset(){
    xyz.clear();
    xyz_raw.clear();
    N_nn.clear();
    nn.clear();
    a2D.clear();
    ts_n.clear();

    this->ego_trans = 0;
    this->ego_rotat = 0;
    this->diff_trans = 0;
    this->diff_rotat = 0;
    this->opti_score = 0;

    this->rotat_b = Eigen::Matrix3d::Identity();
    this->rotat_e = Eigen::Matrix3d::Identity();
    this->trans_b = Eigen::Vector3d::Zero();
    this->trans_e = Eigen::Vector3d::Zero();
    this->root = Eigen::Vector3d::Zero();

    this->time_save_image = 0;
    this->time_save_frame = 0;
    this->time_slam = 0;

    this->map_size_abs = 0;
    this->map_size_rlt = 0;
    this->is_slamed = false;
    this->nb_residual = 0;

    this->trans_b_rlt = glm::vec3(0, 0, 0);
    this->rotat_b_rlt = glm::vec3(0, 0, 0);
    this->trans_e_rlt = glm::vec3(0, 0, 0);
    this->rotat_e_rlt = glm::vec3(0, 0, 0);
  }
};

#endif
