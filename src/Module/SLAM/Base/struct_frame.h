#ifndef FRAME_STRUCT_H
#define FRAME_STRUCT_H

#include <vector>
#include <Eigen/Dense>
#include <glm/glm.hpp>


struct Frame{ //SLAM stuff
  //---------------------------

  Frame();
  void reset();

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
  bool is_slam_made;
  bool is_slam_done;
  int nb_residual;
  int nb_residual_false;
  int map_size_abs;
  int map_size_rlt;
  glm::vec3 trans_b_rlt;
  glm::vec3 rotat_b_rlt;
  glm::vec3 trans_e_rlt;
  glm::vec3 rotat_e_rlt;
  glm::vec3 angle_b;
  glm::vec3 angle_e;

  //---------------------------
};

#endif
