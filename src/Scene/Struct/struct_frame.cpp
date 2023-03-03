#include "struct_frame.h"


Frame::Frame(){
  //---------------------------

  reset();

  //---------------------------
}
void Frame::reset(){
  //---------------------------

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
  this->is_slam_made = false;
  this->is_slam_done = false;
  this->nb_residual = 0;
  this->nb_residual_false = 0;

  this->trans_b_rlt = glm::vec3(0.0f);
  this->rotat_b_rlt = glm::vec3(0.0f);
  this->trans_e_rlt = glm::vec3(0.0f);
  this->rotat_e_rlt = glm::vec3(0.0f);

  //---------------------------
}
