#ifndef SLAM_ASSESSMENT_H
#define SLAM_ASSESSMENT_H

#include "../../../common.h"

class Slam;
class Scene;
class SLAM_optim_gn;


class SLAM_assessment
{
public:
  //Constructor / Destructor
  SLAM_assessment(Slam* slam);
  ~SLAM_assessment();

public:
  bool compute_assessment(Cloud* cloud, int ID);
  bool compute_assessment_abs(Frame* frame, Frame* frame_m1);
  bool compute_assessment_rlt(Cloud* cloud, int i);
  bool compute_assessment_rsd(Frame* frame);

  float AngularDistance(Eigen::Matrix3d& rota, Eigen::Matrix3d& rotb);
  void compute_stat_mean(Cloud* cloud, int ID);

  inline float* get_thres_ego_trans(){return &thres_ego_trans;}
  inline float* get_thres_ego_rotat(){return &thres_ego_rotat;}
  inline float* get_thres_pose_trans(){return &thres_pose_trans;}
  inline float* get_thres_pose_rotat(){return &thres_pose_rotat;}
  inline float* get_thres_optimMinNorm(){return &thres_optimMinNorm;}
  inline int* get_nb_residual_min(){return &nb_residual_min;}
  inline int* get_nb_rlt_previous_mean(){return &nb_rlt_previous_mean;}
  inline int* get_nb_rlt_previous_pose(){return &nb_rlt_previous_pose;}

private:
  Scene* sceneManager;
  SLAM_optim_gn* gnManager;

  float thres_ego_trans;
  float thres_ego_rotat;
  float thres_pose_trans;
  float thres_pose_rotat;
  float thres_optimMinNorm;

  float sum_ego_trans;
  float sum_ego_rotat;
  float sum_diff_trans;
  float sum_diff_rotat;
  float sum_opti_score;

  int nb_rlt_previous_mean;
  int nb_rlt_previous_pose;
  int nb_residual_min;
};


#endif
