#ifndef SLAM_OPTIM_GN_H
#define SLAM_OPTIM_GN_H

#include "../Base/common.h"

#include  <mutex>

class SLAM;
class SLAM_normal;


class SLAM_optim_gn
{
public:
  //Constructor / Destructor
  SLAM_optim_gn(SLAM* slam);
  ~SLAM_optim_gn();

public:
  //Main function
  void update_configuration();
  void optim_GN(Frame* frame, Frame* frame_m1);

  inline double* get_dist_residual_max(){return &dist_residual_max;}
  inline double* get_dist_residual_min(){return &dist_residual_min;}
  inline double get_opti_score(){return X.norm();}
  inline double* get_lambda_location_consistency(){return &lambda_location_consistency;}
  inline double* get_lambda_constant_velocity(){return &lambda_constant_velocity;}
  inline int* get_iter_max(){return &iter_max;}
  inline void set_iter_max(int value){iter_max = value;}
  inline void set_nb_thread(int value){this->nb_thread = value;}

private:
  //Sub-function
  void compute_derivative(Frame* frame);
  void compute_matrices(Frame* frame, Eigen::MatrixXd& J, Eigen::VectorXd& b);
  void compute_constraint(Frame* frame, Frame* frame_m1, Eigen::MatrixXd& J, Eigen::VectorXd& b);
  Eigen::Matrix3d compute_rotationMatrix(double Rx, double Ry, double Rz);

  //Update function
  void update_frame(Frame* frame, Eigen::VectorXd& X);
  void update_keypoints(Frame* frame);

private:
  SLAM_normal* slam_normal;

  vector<Eigen::VectorXd> vec_u;
  Eigen::VectorXd X;
  double lambda_location_consistency;
  double lambda_constant_velocity;
  double dist_residual_max;
  double dist_residual_min;
  int iter_max;
  int nb_thread;
  mutex Mutex;
};


#endif
