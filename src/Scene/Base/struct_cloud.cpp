#include "struct_cloud.h"


Cloud::Cloud(){
  //---------------------------

  this->has_intensity = false;
  this->has_timestamp = false;
  this->pose_T = Eigen::Vector3d::Zero();
  this->pose_R = Eigen::Matrix3d::Zero();
  this->obj_type = "cloud";

  //---------------------------
}

void Cloud::reset(){
  //---------------------------

  this->R.clear();
  this->It.clear();
  this->cosIt.clear();
  this->frame.reset();

  //---------------------------
}
Frame* Cloud::get_frame(){
  //---------------------------

  return &frame;

  //---------------------------
}
