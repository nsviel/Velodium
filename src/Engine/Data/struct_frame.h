#ifndef FRAME_STRUCT_H
#define FRAME_STRUCT_H

#include <vector>
#include <Eigen/Dense>


struct Frame{ //SLAM stuff
  int ID;

  std::vector<Eigen::Vector3f> xyz;
  std::vector<Eigen::Vector3f> xyz_raw;
  std::vector<Eigen::Vector3f> Nptp;
  std::vector<Eigen::Vector3f> NN; // Nearest Neighbor
  std::vector<float> a2D;
  std::vector<float> ts_n;

  //Begin pose
  Eigen::Matrix3f rotat_b;
  Eigen::Vector3f trans_b;

  //End pose
  Eigen::Matrix3f rotat_e;
  Eigen::Vector3f trans_e;

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

    this->ego_trans = 0;
    this->ego_rotat = 0;
    this->diff_trans = 0;
    this->diff_rotat = 0;
    this->opti_score = 0;

    this->rotat_b = Eigen::Matrix3f::Identity();
    this->rotat_e = Eigen::Matrix3f::Identity();
    this->trans_b = Eigen::Vector3f::Zero();
    this->trans_e = Eigen::Vector3f::Zero();

    this->time_save_image = 0;
    this->time_save_frame = 0;
    this->time_slam = 0;

    this->map_size_abs = 0;
    this->map_size_rlt = 0;
    this->is_slamed = false;
    this->nb_residual = 0;

    this->trans_abs = glm::vec3(0, 0, 0);
    this->rotat_abs = glm::vec3(0, 0, 0);
    this->trans_rlt = glm::vec3(0, 0, 0);
    this->rotat_rlt = glm::vec3(0, 0, 0);
  }
};

#endif
