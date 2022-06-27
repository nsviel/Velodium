#ifndef SLAM_OPTIM_GN_H
#define SLAM_OPTIM_GN_H

#include "SLAM_normal.h"

#include "../Cost_function/Cost_function.h"

#include "../../../common.h"

class Slam;


class SLAM_optim_gn
{
public:
  //Constructor / Destructor
  SLAM_optim_gn(Slam* slam);
  ~SLAM_optim_gn();

public:
  void update_configuration();
  void optim_GN(Frame* frame, Frame* frame_m1);

  inline double* get_dist_residual_max(){return &dist_residual_max;}
  inline double get_opti_score(){return X.norm();}
  inline int* get_iter_max(){return &iter_max;}
  inline void set_iter_max(int value){iter_max = value;}
  inline void set_nb_thread(int value){this->nb_thread = value;}

private:
  void compute_derivative(Frame* frame);
  void compute_matrices(Frame* frame, Eigen::MatrixXd& J, Eigen::VectorXd& b);
  void compute_constraint(Frame* frame, Frame* frame_m1, Eigen::MatrixXd& J, Eigen::VectorXd& b);

  void update_parameter(Frame* frame, Eigen::VectorXd& X);
  void update_keypoints(Frame* frame);

  Eigen::Matrix3d compute_rotationMatrix(double Rx, double Ry, double Rz);

private:
  SLAM_normal* normalManager;

  vector<Eigen::VectorXd> vec_u;
  Eigen::VectorXd X;
  double lambda_location;
  double lambda_displace;
  double dist_residual_max;
  int iter_max;
  int nb_thread;
  mutex Mutex;
};


#endif
