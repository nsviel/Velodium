#include "SLAM_assessment.h"

#include "SLAM_map.h"

#include "SLAM.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"
#include "../../../Operation/Transformation/Pose.h"
#include "../../../Specific/Function/fct_math.h"


//Constructor / Destructor
SLAM_assessment::SLAM_assessment(SLAM* slam){
  //---------------------------

  Node_engine* node_engine = slam->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->slam_map = slam->get_slam_map();
  this->poseManager = new Pose();

  this->thres_ego_trans = 2.0f;
  this->thres_ego_rotat = 15.0f;
  this->thres_pose_trans = 5.0f;
  this->thres_pose_rotat = 15.0f;
  this->thres_optimMinNorm = 0.3f;
  this->thres_diff_angle = 5.0f;
  this->thres_time = 500;

  this->nb_rlt_previous_mean = 10;
  this->nb_rlt_previous_pose = 5;
  this->nb_residual_min = 100;

  //---------------------------
}
SLAM_assessment::~SLAM_assessment(){}

//Main function
bool SLAM_assessment::compute_assessment(Collection* collection, int subset_ID, float time){
  Frame* frame_m0 = collection->get_frame_byID(subset_ID);
  Frame* frame_m1 = collection->get_frame_byID(subset_ID-1);
  //---------------------------

  //Check computation time
  bool success_time = compute_assessment_time(time);

  //Check absolute values
  bool success_abs = compute_assessment_abs(frame_m0, frame_m1);

  //Check relative values
  bool success_rlt = compute_assessment_rlt(collection, subset_ID);

  //Check number of residuals
  bool success_rsd = compute_assessment_rsd(frame_m0);

  //Check for any error
  bool success = true;
  if(!success_time || !success_abs || !success_rlt || !success_rsd){
    console.AddLog("error", "[SLAM] Computation failed");
    success = false;
  }

  //---------------------------
  frame_m0->is_slam_done = success;
  this->compute_visibility(collection);
  return success;
}
void SLAM_assessment::compute_visibility(Collection* collection){
  bool slam_failed = false;
  //---------------------------

  for(int i=collection->nb_obj-1; i=0; i--){
    Cloud* cloud = (Cloud*)collection->get_obj(i);
    Frame* frame = &cloud->frame;

    if(frame->is_slam_done == false){
      slam_failed = true;
    }

    if(slam_failed == true){
      cloud->is_visible = false;
    }
  }

  //---------------------------
}

//Specific function
bool SLAM_assessment::compute_assessment_time(float time){
  //---------------------------

  if(time > thres_time){
    string log = "SLAM time too long: " + to_string((int)time) + "/" + to_string(thres_time) + " ms";
    console.AddLog("error", log);
    return false;
  }else{
    return true;
  }

  //---------------------------
}
bool SLAM_assessment::compute_assessment_abs(Frame* frame_m0, Frame* frame_m1){
  bool success = true;
  //---------------------------

  if(frame_m0->ID >= 1 && frame_m0->ID < 5){
    //Test 1: check ego distance
    frame_m0->ego_trans = (frame_m0->trans_e - frame_m0->trans_b).norm();
    if(frame_m0->ego_trans > thres_ego_trans){

      string log = "Ego translation [" + to_string(frame_m0->ego_trans) + "/" + to_string(thres_ego_trans) + "]";
      console.AddLog("error", log);
      success = false;
    }

    //Test 2: Ego angular distance
    frame_m0->ego_rotat = AngularDistance(frame_m0->rotat_b, frame_m0->rotat_e);
    if(frame_m0->ego_rotat > thres_ego_rotat){

      string log = "Ego rotation [" + to_string(frame_m0->ego_rotat) + "/" + to_string(thres_ego_rotat) + "]";
      console.AddLog("error", log);
      success = false;
    }

    //Test 3: check relative distance and orientation between two poses
    if(frame_m0->ID > 2){
      frame_m0->diff_trans = (frame_m0->trans_b - frame_m1->trans_b).norm() + (frame_m0->trans_e - frame_m1->trans_e).norm();
      frame_m0->diff_rotat = AngularDistance(frame_m1->rotat_b, frame_m0->rotat_b) + AngularDistance(frame_m1->rotat_e, frame_m0->rotat_e);
      if(frame_m0->diff_trans > thres_pose_trans){

        string log = "Pose translation [" + to_string(frame_m0->diff_trans) + "/" + to_string(thres_pose_trans) + "]";
        console.AddLog("error", log);
        success = false;
      }
      if(frame_m0->diff_rotat > thres_pose_rotat){

        string log = "Pose rotation [" + to_string(frame_m0->diff_rotat) + "/" + to_string(thres_pose_rotat) + "]";
        console.AddLog("error", log);
        success = false;
      }
    }

    //Test 4: check if ICP has converged
    if(frame_m0->opti_score > thres_optimMinNorm){

      string log = "Optimization score [" + to_string(frame_m0->opti_score) + "/" + to_string(thres_optimMinNorm) + "]";
      console.AddLog("error", log);
      success = false;
    }
  }

  //---------------------------
  return success;
}
bool SLAM_assessment::compute_assessment_rlt(Collection* collection, int subset_ID){
  Frame* frame_m0 = collection->get_frame_byID(subset_ID);
  Frame* frame_m1 = collection->get_frame_byID(subset_ID-1);
  bool success = true;
  //---------------------------

  //Compute relative stats for current frame
  vec3 diff_angle = vec3(0);
  if(frame_m0->ID >= 1){
    frame_m0->ego_trans = (frame_m0->trans_e - frame_m0->trans_b).norm();
    frame_m0->ego_rotat = AngularDistance(frame_m0->rotat_b, frame_m0->rotat_e);
    frame_m0->diff_trans = (frame_m0->trans_b - frame_m1->trans_b).norm() + (frame_m0->trans_e - frame_m1->trans_e).norm();
    frame_m0->diff_rotat = AngularDistance(frame_m1->rotat_b, frame_m0->rotat_b) + AngularDistance(frame_m1->rotat_e, frame_m0->rotat_e);
    vec3 angles_m0 = poseManager->compute_anglesFromTransformationMatrix(frame_m0->rotat_b);
    vec3 angles_m1 = poseManager->compute_anglesFromTransformationMatrix(frame_m1->rotat_b);
    diff_angle = angles_m1 - angles_m0;
  }

  //Make verification tests
  if(frame_m0->ID >= nb_rlt_previous_pose){
    this->compute_stat_mean(collection, subset_ID);

    //Test 1: check ego distance
    if(frame_m0->ego_trans > sum_ego_trans + 1){
      string log = "Ego relative translation [" + to_string(frame_m0->ego_trans) + "/" + to_string(sum_ego_trans) + "]";
      console.AddLog("error", log);
      success = false;
    }

    //Test 2: Ego angular distance
    if(frame_m0->ego_rotat > sum_ego_rotat + 1 && sum_ego_rotat != 0){
      string log = "Ego relative rotation [" + to_string(frame_m0->ego_rotat) + "/" + to_string(sum_ego_rotat) + "]";
      console.AddLog("error", log);
      success = false;
    }

    //Test 3: check relative distance between two poses
    if(frame_m0->diff_trans > sum_diff_trans + 1){
      string log = "Pose relative translation [" + to_string(frame_m0->diff_trans) + "/" + to_string(sum_diff_trans) + "]";
      console.AddLog("error", log);
      success = false;
    }

    //Test 4: check relative rotation between two poses
    if(frame_m0->diff_rotat > sum_diff_rotat + 1 && frame_m0->diff_rotat != 0 && sum_diff_rotat != 0){
      string log = "Pose relative rotation [" + to_string(frame_m0->diff_rotat) + "/" + to_string(sum_diff_rotat) + "]";
      console.AddLog("error", log);
      success = false;
    }

    //Test 5: check if ICP has converged
    if(frame_m0->opti_score > sum_opti_score + 1){
      string log = "Optimization relative score [" + to_string(frame_m0->opti_score) + "/" + to_string(sum_opti_score) + "]";
      console.AddLog("error", log);
      success = false;
    }

    //Test 6: restriction on X & Y rotation axis
    if(diff_angle.x > thres_diff_angle){
      string log = "Relative X axis rotation [" + to_string(diff_angle.x) + "/" + to_string(thres_diff_angle) + "]";
      console.AddLog("error", log);
      success = false;
    }
    if(diff_angle.y > thres_diff_angle){
      string log = "Relative Y axis rotation [" + to_string(diff_angle.y) + "/" + to_string(thres_diff_angle) + "]";
      console.AddLog("error", log);
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
void SLAM_assessment::compute_statistics(Collection* collection, int subset_ID, float duration){
  Cloud* cloud = (Cloud*)collection->get_obj_byID(subset_ID);
  Frame* frame_m0 = collection->get_frame_byID(subset_ID);
  Frame* frame_m1 = collection->get_frame_byID(subset_ID-1);
  slamap* local_map = slam_map->get_local_map();
  //---------------------------

  //Fill stats
  frame_m0->time_slam = duration;
  frame_m0->map_size_abs = local_map->map.size();
  frame_m0->map_size_rlt = local_map->map.size() - local_map->size;
  local_map->size = local_map->map.size();

  //Relative parameters
  vec3 trans_b_rlt, trans_e_rlt;
  if(frame_m1 != nullptr && frame_m0->ID != 0){
    for(int i=0; i<3; i++){
      trans_b_rlt[i] = frame_m0->trans_b(i) - frame_m1->trans_e(i);
      trans_e_rlt[i] = frame_m0->trans_e(i) - frame_m0->trans_b(i);
    }
  }

  vec3 rotat_b_rlt, rotat_e_rlt;
  if(frame_m1 != nullptr && frame_m0->ID != 0){
    vec3 a1_b = poseManager->compute_anglesFromTransformationMatrix(frame_m0->rotat_b);
    vec3 a2_b = poseManager->compute_anglesFromTransformationMatrix(frame_m1->rotat_e);
    rotat_b_rlt = a1_b - a2_b;

    vec3 a1_e = poseManager->compute_anglesFromTransformationMatrix(frame_m0->rotat_e);
    vec3 a2_e = poseManager->compute_anglesFromTransformationMatrix(frame_m0->rotat_b);
    rotat_e_rlt = a1_e - a2_e;
  }

  frame_m0->trans_b_rlt = trans_b_rlt;
  frame_m0->rotat_b_rlt = rotat_b_rlt;
  frame_m0->trans_e_rlt = trans_e_rlt;
  frame_m0->rotat_e_rlt = rotat_e_rlt;
  frame_m0->angle_b = poseManager->compute_anglesFromTransformationMatrix(frame_m0->rotat_b);
  frame_m0->angle_e = poseManager->compute_anglesFromTransformationMatrix(frame_m0->rotat_e);

  //---------------------------
}

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
void SLAM_assessment::compute_stat_mean(Collection* collection, int subset_ID){
  //---------------------------

  //Compute previous frame stat means
  this->sum_ego_trans = 0;
  this->sum_ego_rotat = 0;
  this->sum_diff_trans = 0;
  this->sum_diff_rotat = 0;
  this->sum_opti_score = 0;
  for(int j=1; j<nb_rlt_previous_pose; j++){
    Frame* frame_m = collection->get_frame_byID(subset_ID-j);

    if(frame_m->is_slam_made){
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
