#ifndef SLAM_ASSESSMENT_H
#define SLAM_ASSESSMENT_H

#include "../Base/common.h"

class SLAM;
class Scene;
class SLAM_map;
class Pose;


class SLAM_assessment
{
public:
  //Constructor / Destructor
  SLAM_assessment(SLAM* slam);
  ~SLAM_assessment();

public:
  //Main function
  bool compute_assessment(Collection* collection, int ID, float time);
  void compute_visibility(Collection* collection);

  //Specific function
  bool compute_assessment_time(float time);
  bool compute_assessment_abs(Frame* frame, Frame* frame_m1);
  bool compute_assessment_rlt(Collection* collection, int i);
  bool compute_assessment_rsd(Frame* frame);
  void compute_statistics(Collection* collection, int subset_ID, float duration);

  //Sub-function
  double AngularDistance(Eigen::Matrix3d& rota, Eigen::Matrix3d& rotb);
  void compute_stat_mean(Collection* collection, int ID);

  inline double* get_thres_ego_trans(){return &thres_ego_trans;}
  inline double* get_thres_ego_rotat(){return &thres_ego_rotat;}
  inline double* get_thres_pose_trans(){return &thres_pose_trans;}
  inline double* get_thres_pose_rotat(){return &thres_pose_rotat;}
  inline double* get_thres_optimMinNorm(){return &thres_optimMinNorm;}
  inline double* get_thres_diff_angle(){return &thres_diff_angle;}
  inline int* get_thres_time_ms(){return &thres_time;}
  inline int* get_nb_residual_min(){return &nb_residual_min;}
  inline int* get_nb_rlt_previous_mean(){return &nb_rlt_previous_mean;}
  inline int* get_nb_rlt_previous_pose(){return &nb_rlt_previous_pose;}

private:
  Pose* poseManager;
  Scene* sceneManager;
  SLAM_map* slam_map;

  double thres_ego_trans;
  double thres_ego_rotat;
  double thres_pose_trans;
  double thres_pose_rotat;
  double thres_optimMinNorm;
  double thres_diff_angle;

  double sum_ego_trans;
  double sum_ego_rotat;
  double sum_diff_trans;
  double sum_diff_rotat;
  double sum_opti_score;

  int thres_time;
  int nb_rlt_previous_mean;
  int nb_rlt_previous_pose;
  int nb_residual_min;
};


#endif
