#ifndef SLAM_ASSESSMENT_H
#define SLAM_ASSESSMENT_H

#include "../../../common.h"

class Scene;
class SLAM_optim_gn;


class SLAM_assessment
{
public:
  //Constructor / Destructor
  SLAM_assessment(SLAM_optim_gn* gn);
  ~SLAM_assessment();

public:
  bool compute_assessment(Cloud* cloud, int ID);
  bool compute_assessment_abs(Frame* frame, Frame* frame_m1);
  bool compute_assessment_rlt(Cloud* cloud, int i);

  float AngularDistance(Eigen::Matrix3d& rota, Eigen::Matrix3d& rotb);

  inline float* get_thres_ego_trans(){return &thres_ego_trans;}
  inline float* get_thres_ego_rotat(){return &thres_ego_rotat;}
  inline float* get_thres_pose_trans(){return &thres_pose_trans;}
  inline float* get_thres_pose_rotat(){return &thres_pose_rotat;}
  inline float* get_thres_optimMinNorm(){return &thres_optimMinNorm;}
  inline int* get_nb_residual_min(){return &nb_residual_min;}

private:
  Scene* sceneManager;
  SLAM_optim_gn* gnManager;

  float thres_ego_trans;
  float thres_ego_rotat;
  float thres_pose_trans;
  float thres_pose_rotat;
  float thres_optimMinNorm;

  float rlt_numberMean;
  float rlt_numberPreviousPose;

  int nb_residual_min;
};


#endif
