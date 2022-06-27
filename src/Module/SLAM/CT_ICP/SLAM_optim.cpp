#include "SLAM_optim.h"

#include "SLAM_optim_gn.h"

#include "../Slam.h"


//Constructor / Destructor
SLAM_optim::SLAM_optim(Slam* slam){
  //---------------------------

  this->gnManager = new SLAM_optim_gn(slam);

  //---------------------------
}
SLAM_optim::~SLAM_optim(){}

//Main functions
void SLAM_optim::update_configuration(){
  //---------------------------

  this->with_distorsion = true;
  this->solver_ceres = false;
  this->solver_GN = true;

  //---------------------------
}
void SLAM_optim::compute_distortion(Frame* frame){
  //---------------------------

  if(with_distorsion && frame->ID >= 2){
    Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
    Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
    Eigen::Vector3d trans_b = frame->trans_b;
    Eigen::Vector3d trans_e = frame->trans_e;

    //Distorts the frame
    Eigen::Quaterniond quat_e_inv = quat_e.inverse();
    Eigen::Vector3d trans_e_inv = -1.0 * (quat_e_inv * trans_e);

    for(int i=0; i<frame->xyz.size(); i++){
      float ts_n = frame->ts_n[i];

      Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
      Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

      // Distort Raw Keypoints
      frame->xyz[i] = (R * frame->xyz_raw[i] + t) + (t + trans_e_inv);
    }
  }

  //---------------------------
}
void SLAM_optim::compute_optimization(Frame* frame, Frame* frame_m1){
  //---------------------------

  if(frame->ID > 0){
    this->compute_distortion(frame);

    if(solver_GN){
      gnManager->optim_GN(frame, frame_m1);
    }else if(solver_ceres){
      //ceresManager->optim_test(frame, frame_m1, map);
    }
  }

  //---------------------------
}
