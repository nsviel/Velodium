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
  void optim_GN();

  void frame_update(Frame* frame);
  void frame_update_secured(Frame* frame);
  void frame_distort(Frame* frame);

  inline int* get_iter_max(){return &iter_max;}

private:
  SLAM_normal* slam_normManager;

  int nb_residual;
  int iter_max;
  double* quat_b_ptr;
  double* quat_e_ptr;
  double* trans_b_ptr;
  double* trans_e_ptr;

  // Ceres stuff
  vector<CTPTPR*> vector_residual;
  ceres::LossFunction* loss_function;
};


#endif
