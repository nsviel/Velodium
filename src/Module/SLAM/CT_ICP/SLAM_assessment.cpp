#include "SLAM_assessment.h"

#include "SLAM_optim_gn.h"

#include "../../../Engine/Scene.h"
#include "../../../Specific/fct_maths.h"


//Constructor / Destructor
SLAM_assessment::SLAM_assessment(SLAM_optim_gn* gn){
  //---------------------------

  this->sceneManager = new Scene();

  this->gnManager = gn;

  this->nb_residual_min = 100;

  this->thres_ego_trans = 2.0f;
  this->thres_ego_rotat = 15.0f;
  this->thres_pose_trans = 3.0f;
  this->thres_pose_rotat = 15.0f;
  this->thres_optimMinNorm = 0.2f;

  this->rlt_numberMean = 10.0f;
  this->rlt_numberPreviousPose = 4.0f;

  //---------------------------
}
SLAM_assessment::~SLAM_assessment(){}

//Main function
bool SLAM_assessment::compute_assessment(Cloud* cloud, int ID){
  Frame* frame = sceneManager->get_frame_byID(cloud, ID);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, ID-1);
  //---------------------------

  //Check absolute values
  bool sucess_abs = compute_assessment_abs(frame, frame_m1);

  //Check relative values
  bool sucess_rlt = compute_assessment_rlt(cloud, ID);

  //Check number of residuals
  bool sucess_residual = true;;
  if (frame->nb_residual < nb_residual_min) {
    cout << "[CT_ICP]Error : not enough keypoints selected in ct-icp !" << endl;
    cout << "[CT_ICP]Number_of_residuals : " << frame->nb_residual << endl;
    sucess_residual = false;
  }

  if(!sucess_abs || !sucess_rlt || !sucess_residual){
    string log = "SLAM computation failed";
    console.AddLog("error", log);
  }

  //---------------------------
  return true;
}
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
bool SLAM_assessment::compute_assessment_rlt(Cloud* cloud, int ID){
  Frame* frame_m0 = sceneManager->get_frame_byID(cloud, ID);
  bool sucess = true;
  //---------------------------

  if(frame_m0->ID >= rlt_numberPreviousPose){
    Frame* frame_m1 = sceneManager->get_frame_byID(cloud, ID-1);

    //Compute previsou frame stat means
    float sum_ego_trans = 0;
    float sum_ego_rotat = 0;
    float sum_diff_trans = 0;
    float sum_diff_rotat = 0;
    float sum_opti_score = 0;
    for(int j=1; j<rlt_numberPreviousPose; j++){
      Frame* frame_m = sceneManager->get_frame_byID(cloud, ID-j);

      if(frame_m->is_slamed){
        sum_ego_trans += frame_m->ego_trans;
        sum_ego_rotat += frame_m->ego_rotat;
        sum_diff_trans += frame_m->diff_trans;
        sum_diff_rotat += frame_m->diff_rotat;
        sum_opti_score += frame_m->opti_score;
      }

    }
    sum_ego_trans = rlt_numberMean * sum_ego_trans / rlt_numberPreviousPose;
    sum_ego_rotat = rlt_numberMean * sum_ego_rotat / rlt_numberPreviousPose;
    sum_diff_trans = rlt_numberMean * sum_diff_trans / rlt_numberPreviousPose;
    sum_diff_rotat = rlt_numberMean * sum_diff_rotat / rlt_numberPreviousPose;
    sum_opti_score = rlt_numberMean * sum_opti_score / rlt_numberPreviousPose;

    //Test 1: check ego distance
    frame_m0->ego_trans = (frame_m0->trans_e - frame_m0->trans_b).norm();
    if(frame_m0->ego_trans > sum_ego_trans){
      cout<<"[error] Ego relative translation too important ";
      cout<<"["<<frame_m0->ego_trans<<"/"<<sum_ego_trans<<"]"<<endl;
      sucess = false;
    }

    //Test 2: Ego angular distance
    frame_m0->ego_rotat = AngularDistance(frame_m0->rotat_b, frame_m0->rotat_e);
    if(frame_m0->ego_rotat > sum_ego_rotat && sum_ego_rotat != 0){
      cout<<"[error] Ego relative rotation too important ";
      cout<<"["<<frame_m0->ego_rotat<<"/"<<sum_ego_rotat<<"]"<<endl;
      sucess = false;
    }

    //Test 3: check relative distance between two poses
    frame_m0->diff_trans = (frame_m0->trans_b - frame_m1->trans_b).norm() + (frame_m0->trans_e - frame_m1->trans_e).norm();
    if(frame_m0->diff_trans > sum_diff_trans){
      cout<<"[error] Pose relative translation too important ";
      cout<<"["<<frame_m0->diff_trans<<"/"<<sum_diff_trans<<"]"<<endl;
      sucess = false;
    }

    //Test 4: check relative rotation between two poses
    frame_m0->diff_rotat = AngularDistance(frame_m1->rotat_b, frame_m0->rotat_b) + AngularDistance(frame_m1->rotat_e, frame_m0->rotat_e);
    if(frame_m0->diff_rotat > sum_diff_rotat && frame_m0->diff_rotat != 0 && sum_diff_rotat != 0){
      cout<<"[error] Pose relative rotation too important ";
      cout<<"["<<frame_m0->diff_rotat<<"/"<<sum_diff_rotat<<"]"<<endl;
      sucess = false;
    }

    //Test 5: check if ICP has converged
    frame_m0->opti_score = gnManager->get_opti_score();
    if(frame_m0->opti_score > sum_opti_score){
      cout<<"[error] Optimization relative score too important ";
      cout<<"["<<frame_m0->opti_score<<"/"<<sum_opti_score<<"]"<<endl;
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
