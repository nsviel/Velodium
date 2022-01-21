#include "SLAM_assessment.h"

#include "SLAM_optim_gn.h"

#include "../../../Specific/fct_maths.h"


//Constructor / Destructor
SLAM_assessment::SLAM_assessment(SLAM_optim_gn* gn){
  //---------------------------

  this->gnManager = gn;

  this->thres_ego_trans = 2.0f;
  this->thres_ego_rotat = 15;
  this->thres_pose_trans = 3.0f;
  this->thres_pose_rotat = 15.0f;
  this->thres_optimMinNorm = 0.1;

  //---------------------------
}
SLAM_assessment::~SLAM_assessment(){}

//Main function
bool SLAM_assessment::compute_assessment_abs(Frame* frame_m0, Frame* frame_m1){
  bool sucess = true;
  //---------------------------

  if(frame_m0->ID >= 1 && frame_m0->ID < 5){
    //Test 1: check ego distance
    frame_m0->ego_trans = (frame_m0->trans_e - frame_m0->trans_b).norm();
    if(frame_m0->ego_trans > thres_ego_trans){
      cout<<"[error] Ego translation too important ";
      cout<<"["<<frame_m0->ego_trans<<"/"<<thres_ego_trans<<"]"<<endl;
      sucess = false;
    }

    //Test 2: Ego angular distance
    frame_m0->ego_rotat = AngularDistance(frame_m0->rotat_b, frame_m0->rotat_e);
    if(frame_m0->ego_rotat > thres_ego_rotat){
      cout<<"[error] Ego rotation too important ";
      cout<<"["<<frame_m0->ego_rotat<<"/"<<thres_ego_rotat<<"]"<<endl;
      sucess = false;
    }

    //Test 3: check relative distance and orientation between two poses
    if(frame_m0->ID > 2){
      frame_m0->diff_trans = (frame_m0->trans_b - frame_m1->trans_b).norm() + (frame_m0->trans_e - frame_m1->trans_e).norm();
      frame_m0->diff_rotat = AngularDistance(frame_m1->rotat_b, frame_m0->rotat_b) + AngularDistance(frame_m1->rotat_e, frame_m0->rotat_e);
      if(frame_m0->diff_trans > thres_pose_trans){
        cout<<"[error] Pose translation too important ";
        cout<<"["<<frame_m0->diff_trans<<"/"<<thres_pose_trans<<"]"<<endl;
        sucess = false;
      }
      if(frame_m0->diff_rotat > thres_pose_rotat){
        cout<<"[error] Pose rotation too important ";
        cout<<"["<<frame_m0->diff_rotat<<"/"<<thres_pose_rotat<<"]"<<endl;
        sucess = false;
      }
    }

    //Test 4: check if ICP has converged
    frame_m0->opti_score = gnManager->get_opti_score();
    if(frame_m0->opti_score > thres_optimMinNorm){
      cout<<"[error] Optimization score too important ";
      cout<<"["<<frame_m0->opti_score<<"/"<<thres_optimMinNorm<<"]"<<endl;
      sucess = false;
    }
  }

  //---------------------------
  return sucess;
}
bool SLAM_assessment::compute_assessment_rlt(Cloud* cloud, int i){
  Frame* frame_m0 = &cloud->subset[i].frame;
  bool sucess = true;
  //---------------------------

  if(frame_m0->ID >= 5){
    Frame* frame_m1 = &cloud->subset[i-1].frame;

    //Compute previsou frame stat means
    float sum_ego_trans = 0;
    float sum_ego_rotat = 0;
    float sum_diff_trans = 0;
    float sum_diff_rotat = 0;
    float sum_opti_score = 0;
    for(int j=1; j<5; j++){
      Frame* frame_m = &cloud->subset[i-j].frame;
      sum_ego_trans += frame_m->ego_trans;
      sum_ego_rotat += frame_m->ego_rotat;
      sum_diff_trans += frame_m->diff_trans;
      sum_diff_rotat += frame_m->diff_rotat;
      sum_opti_score += frame_m->opti_score;
    }
    sum_ego_trans = 3 * sum_ego_trans / 5;
    sum_ego_rotat = 3 * sum_ego_rotat / 5;
    sum_diff_trans = 3 * sum_diff_trans / 5;
    sum_diff_rotat = 3 * sum_diff_rotat / 5;
    sum_opti_score = 3 * sum_opti_score / 5;

    //Test 1: check ego distance
    frame_m0->ego_trans = (frame_m0->trans_e - frame_m0->trans_b).norm();
    if(frame_m0->ego_trans > sum_ego_trans){
      cout<<"[error] Ego translation too important ";
      cout<<"["<<frame_m0->ego_trans<<"/"<<thres_ego_trans<<"]"<<endl;
      sucess = false;
    }

    //Test 2: Ego angular distance
    frame_m0->ego_rotat = AngularDistance(frame_m0->rotat_b, frame_m0->rotat_e);
    if(frame_m0->ego_rotat > sum_ego_rotat){
      cout<<"[error] Ego rotation too important ";
      cout<<"["<<frame_m0->ego_rotat<<"/"<<thres_ego_rotat<<"]"<<endl;
      sucess = false;
    }

    //Test 3: check relative distance and orientation between two poses
    if(frame_m0->ID > 2){
      frame_m0->diff_trans = (frame_m0->trans_b - frame_m1->trans_b).norm() + (frame_m0->trans_e - frame_m1->trans_e).norm();
      frame_m0->diff_rotat = AngularDistance(frame_m1->rotat_b, frame_m0->rotat_b) + AngularDistance(frame_m1->rotat_e, frame_m0->rotat_e);
      if(frame_m0->diff_trans > sum_diff_trans){
        cout<<"[error] Pose translation too important ";
        cout<<"["<<frame_m0->diff_trans<<"/"<<thres_pose_trans<<"]"<<endl;
        sucess = false;
      }
      if(frame_m0->diff_rotat > sum_diff_rotat){
        cout<<"[error] Pose rotation too important ";
        cout<<"["<<frame_m0->diff_rotat<<"/"<<thres_pose_rotat<<"]"<<endl;
        sucess = false;
      }
    }

    //Test 4: check if ICP has converged
    frame_m0->opti_score = gnManager->get_opti_score();
    if(frame_m0->opti_score > sum_opti_score){
      cout<<"[error] Optimization score too important ";
      cout<<"["<<frame_m0->opti_score<<"/"<<thres_optimMinNorm<<"]"<<endl;
      sucess = false;
    }
  }

  //---------------------------
  return sucess;
}

//Subfunctions
float SLAM_assessment::AngularDistance(Eigen::Matrix3d &rota, Eigen::Matrix3d &rotb){
  float norm = ((rota * rotb.transpose()).trace() - 1) / 2;
  norm = std::acos(norm) * 180 / M_PI;
  if(isnan(norm)){
    norm = 0;
  }
  return norm;
}
