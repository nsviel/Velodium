#include "SLAM_optim_gn.h"

#include "../Slam.h"

#include "../../../Specific/fct_terminal.h"
#include "../../../Specific/fct_transtypage.h"
#include "../../../Specific/fct_maths.h"


//Constructor / Destructor
SLAM_optim_gn::SLAM_optim_gn(Slam* slam){
  //---------------------------

  this->normalManager = slam->get_slam_normal();

  //---------------------------
  this->update_configuration();
}
SLAM_optim_gn::~SLAM_optim_gn(){}

//Main functions
void SLAM_optim_gn::update_configuration(){
  //---------------------------

  this->iter_max = 5;
  this->nb_thread = 8;
  this->PTP_distance_max = 0.5f;
  this->lambda_location = 0.001;
  this->lambda_displace =  0.001;
  X_old= Eigen::VectorXf(0);

  //---------------------------
}
void SLAM_optim_gn::optim_GN(Frame* frame_m0, Frame* frame_m1, voxelMap* map){
  // X [0-2]: rotat_b
  // X [3-5]: trans_b
  // X [6-8]: rotat_e
  // X [9-11]: trans_e
  this->X = X_old;
  //---------------------------

  for (int iter=0; iter < iter_max; iter++){
    //Initialization
    Eigen::MatrixXf J = Eigen::MatrixXf::Zero(12, 12);
    Eigen::VectorXf b = Eigen::VectorXf::Zero(12);

    //Derive residuals
    normalManager->compute_normal(frame_m0, map);
    this->compute_residual(frame_m0, J, b);
    this->compute_constraint(frame_m0, frame_m1, J, b);

    //Solve
    X = J.ldlt().solve(b);

    //Update
    this->update_frame(frame_m0, X);
    this->update_keypoints(frame_m0);
  }
  X_old = X;

  //---------------------------
}

//Subfunctions
void SLAM_optim_gn::compute_residual(Frame* frame, Eigen::MatrixXf& J, Eigen::VectorXf& b){
  //---------------------------

  this->compute_residual_parameter(frame);
  this->compute_residual_apply(frame, J, b);

  //---------------------------
}
void SLAM_optim_gn::compute_residual_parameter(Frame* frame){
  vec_u.clear(); vec_u.resize(frame->xyz.size());
  frame->nb_residual = 0;
  //---------------------------

  //compute residual parameters
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d point = frame->xyz[i];
    Eigen::Vector3d point_raw = frame->xyz_raw[i];
    Eigen::Vector3d normal = frame->Nptp[i];
    Eigen::Vector3d iNN = frame->NN[i];

    //Compute point-to-plane distance
    double PTP_distance = 0;
    for(int j=0; j<3; j++){
      PTP_distance += normal[j] * (point[j] - iNN[j]);
    }

    double a2D = frame->a2D[i];
    if(abs(PTP_distance) < PTP_distance_max && a2D != -1){
      //Compute normal of the iNN point
      Eigen::Vector3d iNN_N = a2D * a2D * normal;

      //Compute residual
      double residual = 0;
      for(int j=0; j<3; j++){
        residual += (point[j] - iNN[j]) * iNN_N[j];
      }
      frame->nb_residual++;

      //Compute parameters
      double ts_n = frame->ts_n[i];
      Eigen::Vector3d origin_b = frame->rotat_b * point_raw;
      Eigen::Vector3d origin_e = frame->rotat_e * point_raw;

      double cbx = (1 - ts_n) * (origin_b[1] * iNN_N[2] - origin_b[2] * iNN_N[1]);
      double cby = (1 - ts_n) * (origin_b[2] * iNN_N[0] - origin_b[0] * iNN_N[2]);
      double cbz = (1 - ts_n) * (origin_b[0] * iNN_N[1] - origin_b[1] * iNN_N[0]);

      double nbx = (1 - ts_n) * iNN_N[0];
      double nby = (1 - ts_n) * iNN_N[1];
      double nbz = (1 - ts_n) * iNN_N[2];

      double cex = ts_n * (origin_e[1] * iNN_N[2] - origin_e[2] * iNN_N[1]);
      double cey = ts_n * (origin_e[2] * iNN_N[0] - origin_e[0] * iNN_N[2]);
      double cez = ts_n * (origin_e[0] * iNN_N[1] - origin_e[1] * iNN_N[0]);

      double nex = ts_n * iNN_N[0];
      double ney = ts_n * iNN_N[1];
      double nez = ts_n * iNN_N[2];

      //Store results
      Eigen::VectorXf u(13);
      u << cbx, cby, cbz, nbx, nby, nbz, cex, cey, cez, nex, ney, nez, residual;
      vec_u[i] = u;
    }
  }

  //---------------------------
}
void SLAM_optim_gn::compute_residual_apply(Frame* frame, Eigen::MatrixXf& J, Eigen::VectorXf& b){
  //---------------------------

  //Apply parameters & residuals
  for(int i=0; i<frame->xyz.size(); i++){
    if(vec_u[i].size() != 0){
      for (int j = 0; j < 12; j++) {
        //Jacobian
        for (int k = 0; k < 12; k++) {
          J(j, k) = J(j, k) + vec_u[i][j] * vec_u[i][k];
        }

        //b vector
        b(j) = b(j) - vec_u[i][j] * vec_u[i][12];
      }
    }
  }

  // Normalize equation
  #pragma omp parallel for num_threads(nb_thread)
  for (int i=0; i < 12; i++) {
    for (int j=0; j < 12; j++) {
        J(i, j) /= frame->nb_residual;
    }
    b(i) /= frame->nb_residual;
  }

  //---------------------------
}
void SLAM_optim_gn::compute_constraint(Frame* frame_m0, Frame* frame_m1, Eigen::MatrixXf& J, Eigen::VectorXf& b){
  //---------------------------

  //Add constraints in trajectory
  if(frame_m0->ID > 1){
    Eigen::Vector3d trans_b_m0 = frame_m0->trans_b;
    Eigen::Vector3d trans_e_m0 = frame_m0->trans_e;
    Eigen::Vector3d trans_b_m1 = frame_m1->trans_b;
    Eigen::Vector3d trans_e_m1 = frame_m1->trans_e;

    Eigen::Vector3d diff_traj = trans_b_m0 - trans_e_m1;
    J(3, 3) = J(3, 3) + lambda_location;
    J(4, 4) = J(4, 4) + lambda_location;
    J(5, 5) = J(5, 5) + lambda_location;
    b(3) = b(3) - lambda_location * diff_traj(0);
    b(4) = b(4) - lambda_location * diff_traj(1);
    b(5) = b(5) - lambda_location * diff_traj(2);

    Eigen::Vector3d diff_ego = trans_e_m0 - trans_b_m0 - trans_e_m1 + trans_b_m1;
    J(9, 9)   = J(9, 9)   + lambda_displace;
    J(10, 10) = J(10, 10) + lambda_displace;
    J(11, 11) = J(11, 11) + lambda_displace;
    b(9)  = b(9)  - lambda_displace * diff_ego(0);
    b(10) = b(10) - lambda_displace * diff_ego(1);
    b(11) = b(11) - lambda_displace * diff_ego(2);
  }

  //---------------------------
}
Eigen::Matrix3d SLAM_optim_gn::compute_rotationMatrix(double rx, double ry, double rz){
  Eigen::Matrix3d R;
  //---------------------------

  R(0, 0) = cos(rz) * cos(ry);
  R(0, 1) = -sin(rz) * cos(rx) + cos(rz) * sin(ry) * sin(rx);
  R(0, 2) = sin(rz) * sin(rx) + cos(rz) * sin(ry) * cos(rx);
  R(1, 0) = sin(rz) * cos(ry);
  R(1, 1) = cos(rz) * cos(rx) + sin(rz) * sin(ry) * sin(rx);
  R(1, 2) = -cos(rz) * sin(rx) + sin(rz) * sin(ry) * cos(rx);
  R(2, 0) = -sin(ry);
  R(2, 1) = cos(ry) * sin(rx);
  R(2, 2) = cos(ry) * cos(rx);

  //---------------------------
  return R;
}

//Update functions
void SLAM_optim_gn::update_frame(Frame* frame, Eigen::VectorXf& X){
  //---------------------------

  //Retrieve parameters
  Eigen::Matrix3d rotat_b = compute_rotationMatrix(X(0), X(1), X(2));
  Eigen::Vector3d trans_b = Eigen::Vector3d(X(3), X(4), X(5));

  Eigen::Matrix3d rotat_e = compute_rotationMatrix(X(6), X(7), X(8));
  Eigen::Vector3d trans_e = Eigen::Vector3d(X(9), X(10), X(11));

  //Update parameters
  frame->rotat_b *= rotat_b;
  frame->trans_b += trans_b;

  frame->rotat_e *= rotat_e;
  frame->trans_e += trans_e;

  //---------------------------
}
void SLAM_optim_gn::update_keypoints(Frame* frame){
  //---------------------------

  //Update keypoints
  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d point_raw = frame->xyz_raw[i];
    Eigen::Vector3d& point = frame->xyz[i];
    double ts_n = frame->ts_n[i];

    Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    point = R * point_raw + t;
  }

  //---------------------------
}
