#ifdef USE_CERES
#ifndef SLAM_OPTIM_CERES_H
#define SLAM_OPTIM_CERES_H

#include "SLAM_normal.h"

#include "../Cost_function/Cost_function.h"

#include "../../../common.h"

#include <ceres/internal/eigen.h>
#include <ceres/ceres.h>
#include <ceres/rotation.h>


class SLAM_optim_ceres
{
public:
  //Constructor / Destructor
  SLAM_optim_ceres(SLAM_normal* normal);
  ~SLAM_optim_ceres();

public:
  void optim_CERES(Frame* frame, Frame* frame_m1);
  void optim_test(Frame* frame, Frame* frame_m1, voxelMap* map);
  void optim_test_secured(Frame* frame, Frame* frame_m1, voxelMap* map);

  void frame_update(Frame* frame);
  void frame_update_secured(Frame* frame);
  void frame_distort(Frame* frame);

  inline int* get_iter_max(){return &iter_max;}
  inline void set_iter_max(int value){iter_max = value;}

private:
  //Ceres functions
  void ceres_computePointResidual(Frame* frame);
  void ceres_addParameter(ceres::Problem* problem, Frame* frame);
  void ceres_addPointResidual(ceres::Problem* ceres_problem, Frame* frame);
  void ceres_addConstraintResidual(ceres::Problem* ceres_problem, Frame* frame, Frame* frame_m1, int number_of_residuals);
  void ceres_solve(ceres::Problem* ceres_problem);

private:
  SLAM_normal* normalManager;

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
#endif
