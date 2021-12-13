#ifndef SLAM_OPTIM_GN_H
#define SLAM_OPTIM_GN_H

#include "SLAM_normal.h"

#include "../Cost_function/Cost_function.h"

#include "../../../common.h"

#include <ceres/internal/eigen.h>
#include <ceres/ceres.h>
#include <ceres/rotation.h>


class SLAM_optim_gn
{
public:
  //Constructor / Destructor
  SLAM_optim_gn();
  ~SLAM_optim_gn();

public:
  void optim_GN(Frame* frame, Frame* frame_m1, voxelMap* map);

  inline int* get_iter_max(){return &iter_max;}
  inline void set_iter_max(int value){iter_max = value;}
  inline void set_nb_thread(int value){this->nb_thread = value;}

private:
  void compute_constraints(Frame* frame, Frame* frame_m1, Eigen::MatrixXd& J, Eigen::VectorXd& b);
  void compute_residuals(Frame* frame, Eigen::MatrixXd& J, Eigen::VectorXd& b);

  void update_frame(Frame* frame, Eigen::VectorXd& X);
  void update_keypoints(Frame* frame);

  Eigen::Matrix3d compute_rotationMatrix(float Rx, float Ry, float Rz);

private:
  SLAM_normal* normalManager;

  int iter_max;
  int nb_thread;
  int nb_residual;
};


#endif
