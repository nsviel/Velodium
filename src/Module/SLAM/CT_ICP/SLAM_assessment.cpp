#include "SLAM_assessment.h"

#include "SLAM_optim_gn.h"

#include "../Slam.h"

#include "../../../Engine/Engine_node.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Operation/Transformation/Transforms.h"
#include "../../../Specific/fct_maths.h"


//Constructor / Destructor
SLAM_assessment::SLAM_assessment(Slam* slam){
  //---------------------------

  Engine_node* node_engine = slam->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->gnManager = slam->get_slam_gn();

  this->nb_residual_min = 100;

  this->thres_ego_trans = 2.0f;
  this->thres_ego_rotat = 15.0f;
  this->thres_pose_trans = 5.0f;
  this->thres_pose_rotat = 15.0f;
  this->thres_optimMinNorm = 0.3f;
  this->thres_diff_angle = 3.0f; // A ajouter aux configuration SLAM_parameter et gui

  this->nb_rlt_previous_mean = 10;
  this->nb_rlt_previous_pose = 5;

  //---------------------------
}
SLAM_assessment::~SLAM_assessment(){}

//Main function
bool SLAM_assessment::compute_assessment(Cloud* cloud, int subset_ID){
  Frame* frame_m0 = sceneManager->get_frame_byID(cloud, subset_ID);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, subset_ID-1);
  //---------------------------

  //Check absolute values
  bool success_abs = compute_assessment_abs(frame_m0, frame_m1);

  //Check relative values
  bool success_rlt = compute_assessment_rlt(cloud, subset_ID);

  //Check number of residuals
  bool success_rsd = compute_assessment_rsd(frame_m0);

  //Check for any error
  if(!success_abs || !success_rlt || !success_rsd){
    string log = "SLAM computation failed";
    console.AddLog("error", log);
    return false;
  }

  //---------------------------
  return true;
}
bool SLAM_assessment::compute_assessment_abs(Frame* frame_m0, Frame* frame_m1){
  bool success = true;
  //---------------------------

  if(frame_m0->ID >= 1 && frame_m0->ID < 5){
    //Test 1: check ego distance
    frame_m0->ego_trans = (frame_m0->trans_e - frame_m0->trans_b).norm();
    if(frame_m0->ego_trans > thres_ego_trans){
      cout<<"[error] Ego translation too important ";
      cout<<"["<<frame_m0->ego_trans<<"/"<<thres_ego_trans<<"]"<<endl;
      success = false;
    }

    //Test 2: Ego angular distance
    frame_m0->ego_rotat = AngularDistance(frame_m0->rotat_b, frame_m0->rotat_e);
    if(frame_m0->ego_rotat > thres_ego_rotat){
      cout<<"[error] Ego rotation too important ";
      cout<<"["<<frame_m0->ego_rotat<<"/"<<thres_ego_rotat<<"]"<<endl;
      success = false;
    }

    //Test 3: check relative distance and orientation between two poses
    if(frame_m0->ID > 2){
      frame_m0->diff_trans = (frame_m0->trans_b - frame_m1->trans_b).norm() + (frame_m0->trans_e - frame_m1->trans_e).norm();
      frame_m0->diff_rotat = AngularDistance(frame_m1->rotat_b, frame_m0->rotat_b) + AngularDistance(frame_m1->rotat_e, frame_m0->rotat_e);
      if(frame_m0->diff_trans > thres_pose_trans){
        cout<<"[error] Pose translation too important ";
        cout<<"["<<frame_m0->diff_trans<<"/"<<thres_pose_trans<<"]"<<endl;
        success = false;
      }
      if(frame_m0->diff_rotat > thres_pose_rotat){
        cout<<"[error] Pose rotation too important ";
        cout<<"["<<frame_m0->diff_rotat<<"/"<<thres_pose_rotat<<"]"<<endl;
        success = false;
      }
    }

    //Test 4: check if ICP has converged
    frame_m0->opti_score = gnManager->get_opti_score();
    if(frame_m0->opti_score > thres_optimMinNorm){
      cout<<"[error] Optimization score too important ";
      cout<<"["<<frame_m0->opti_score<<"/"<<thres_optimMinNorm<<"]"<<endl;
      success = false;
    }
  }

  //---------------------------
  return success;
}
bool SLAM_assessment::compute_assessment_rlt(Cloud* cloud, int subset_ID){
  Frame* frame_m0 = sceneManager->get_frame_byID(cloud, subset_ID);
  Frame* frame_m1 = sceneManager->get_frame_byID(cloud, subset_ID-1);
  Transforms transformManager;
  bool success = true;
  //---------------------------

  //Compute relative stats for current frame
  vec3 diff_angle = vec3(0);
  if(frame_m0->ID >= 1){
    frame_m0->ego_trans = (frame_m0->trans_e - frame_m0->trans_b).norm();
    frame_m0->ego_rotat = AngularDistance(frame_m0->rotat_b, frame_m0->rotat_e);
    frame_m0->diff_trans = (frame_m0->trans_b - frame_m1->trans_b).norm() + (frame_m0->trans_e - frame_m1->trans_e).norm();
    frame_m0->diff_rotat = AngularDistance(frame_m1->rotat_b, frame_m0->rotat_b) + AngularDistance(frame_m1->rotat_e, frame_m0->rotat_e);
    frame_m0->opti_score = gnManager->get_opti_score();
    vec3 angles_m0 = transformManager.compute_anglesFromTransformationMatrix(frame_m0->rotat_b);
    vec3 angles_m1 = transformManager.compute_anglesFromTransformationMatrix(frame_m1->rotat_b);
    diff_angle = angles_m1 - angles_m0;
  }

  //Make verification tests
  if(frame_m0->ID >= nb_rlt_previous_pose){
    this->compute_stat_mean(cloud, subset_ID);

    //Test 1: check ego distance
    if(frame_m0->ego_trans > sum_ego_trans + 1){
      cout<<"[error] Ego relative translation too important ";
      cout<<"["<<frame_m0->ego_trans<<"/"<<sum_ego_trans<<"]"<<endl;
      success = false;
    }

    //Test 2: Ego angular distance
    if(frame_m0->ego_rotat > sum_ego_rotat + 1 && sum_ego_rotat != 0){
      cout<<"[error] Ego relative rotation too important ";
      cout<<"["<<frame_m0->ego_rotat<<"/"<<sum_ego_rotat<<"]"<<endl;
      success = false;
    }

    //Test 3: check relative distance between two poses
    if(frame_m0->diff_trans > sum_diff_trans + 1){
      cout<<"[error] Pose relative translation too important ";
      cout<<"["<<frame_m0->diff_trans<<"/"<<sum_diff_trans<<"]"<<endl;
      success = false;
    }

    //Test 4: check relative rotation between two poses
    if(frame_m0->diff_rotat > sum_diff_rotat + 1 && frame_m0->diff_rotat != 0 && sum_diff_rotat != 0){
      cout<<"[error] Pose relative rotation too important ";
      cout<<"["<<frame_m0->diff_rotat<<"/"<<sum_diff_rotat<<"]"<<endl;
      success = false;
    }

    //Test 5: check if ICP has converged
    if(frame_m0->opti_score > sum_opti_score + 1){
      cout<<"[error] Optimization relative score too important ";
      cout<<"["<<frame_m0->opti_score<<"/"<<sum_opti_score<<"]"<<endl;
      success = false;
    }

    //Test 6: restriction on X & Y rotation axis
    if(diff_angle.x > thres_diff_angle){
      cout<<"[error] Relative X axis rotation angle too high ";
      cout<<"["<<diff_angle.x<<"/"<<thres_diff_angle<<"]"<<endl;
      success = false;
    }
    if(diff_angle.y > thres_diff_angle){
      cout<<"[error] Relative Y axis rotation angle too high ";
      cout<<"["<<diff_angle.y<<"/"<<thres_diff_angle<<"]"<<endl;
      success = false;
    }
  }

  //---------------------------
  return success;
}
bool SLAM_assessment::compute_assessment_rsd(Frame* frame){
  //---------------------------

  if(frame->ID != 0 && frame->nb_residual < nb_residual_min){
    string log = "Not enough keypoints: " + to_string(frame->nb_residual) + "/" + to_string(nb_residual_min);
    console.AddLog("error", log);
    return false;
  }

  //---------------------------
  return true;
}

//Mettre des assessment sur la rotation max selon X et Y

//Subfunctions
double SLAM_assessment::AngularDistance(Eigen::Matrix3d &rota, Eigen::Matrix3d &rotb){
  //---------------------------

  double norm = ((rota * rotb.transpose()).trace() - 1) / 2;
  norm = std::acos(norm) * 180 / M_PI;
  if(isnan(norm)){
    norm = 0;
  }

  //---------------------------
  return norm;
}
void SLAM_assessment::compute_stat_mean(Cloud* cloud, int subset_ID){
  //---------------------------

  //Compute previous frame stat means
  this->sum_ego_trans = 0;
  this->sum_ego_rotat = 0;
  this->sum_diff_trans = 0;
  this->sum_diff_rotat = 0;
  this->sum_opti_score = 0;
  for(int j=1; j<nb_rlt_previous_pose; j++){
    Frame* frame_m = sceneManager->get_frame_byID(cloud, subset_ID-j);

    if(frame_m->is_slamed){
      sum_ego_trans += frame_m->ego_trans;
      sum_ego_rotat += frame_m->ego_rotat;
      sum_diff_trans += frame_m->diff_trans;
      sum_diff_rotat += frame_m->diff_rotat;
      sum_opti_score += frame_m->opti_score;
    }

  }
  this->sum_ego_trans = nb_rlt_previous_mean * sum_ego_trans / nb_rlt_previous_pose;
  this->sum_ego_rotat = nb_rlt_previous_mean * sum_ego_rotat / nb_rlt_previous_pose;
  this->sum_diff_trans = nb_rlt_previous_mean * sum_diff_trans / nb_rlt_previous_pose;
  this->sum_diff_rotat = nb_rlt_previous_mean * sum_diff_rotat / nb_rlt_previous_pose;
  this->sum_opti_score = nb_rlt_previous_mean * sum_opti_score / nb_rlt_previous_pose;

  //---------------------------
}
