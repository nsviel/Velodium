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

  //---------------------------
}
void SLAM_optim_gn::optim_GN(Frame* frame, Frame* frame_m1, voxelMap* map){
  // X [0-2]: rotat_b
  // X [3-5]: trans_b
  // X [6-8]: rotat_e
  // X [9-11]: trans_e
  this->X = Eigen::VectorXf(0);
  //---------------------------

  for (int iter=0; iter < iter_max; iter++){
    //Initialization
    Eigen::MatrixXf J = Eigen::MatrixXf::Zero(12, 12);
    Eigen::VectorXf b = Eigen::VectorXf::Zero(12);

    //Derive residuals
    normalManager->compute_normal(frame, map);
    this->compute_residual(frame, J, b);
    this->compute_constraint(frame, frame_m1, J, b);

    //Solve
    X = J.ldlt().solve(b);

    //Update
    this->update_frame(frame, X);
    this->update_keypoints(frame);
  }

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
    Eigen::Vector3f point = frame->xyz[i];
    Eigen::Vector3f point_raw = frame->xyz_raw[i];
    Eigen::Vector3f normal = frame->Nptp[i];
    Eigen::Vector3f iNN = frame->NN[i];

    //Compute point-to-plane distance
    float PTP_distance = 0;
    for(int j=0; j<3; j++){
      PTP_distance += normal[j] * (point[j] - iNN[j]);
    }

    float a2D = frame->a2D[i];
    if(abs(PTP_distance) < PTP_distance_max && a2D != -1){
      //Compute normal of the iNN point
      Eigen::Vector3f iNN_N = a2D * a2D * normal;

      //Compute residual
      float residual = 0;
      for(int j=0; j<3; j++){
        residual += (point[j] - iNN[j]) * iNN_N[j];
      }
      frame->nb_residual++;

      //Compute parameters
      float ts_n = frame->ts_n[i];
      Eigen::Vector3f origin_b = frame->rotat_b * point_raw;
      Eigen::Vector3f origin_e = frame->rotat_e * point_raw;

      float cbx = (1 - ts_n) * (origin_b[1] * iNN_N[2] - origin_b[2] * iNN_N[1]);
      float cby = (1 - ts_n) * (origin_b[2] * iNN_N[0] - origin_b[0] * iNN_N[2]);
      float cbz = (1 - ts_n) * (origin_b[0] * iNN_N[1] - origin_b[1] * iNN_N[0]);

      float nbx = (1 - ts_n) * iNN_N[0];
      float nby = (1 - ts_n) * iNN_N[1];
      float nbz = (1 - ts_n) * iNN_N[2];

      float cex = ts_n * (origin_e[1] * iNN_N[2] - origin_e[2] * iNN_N[1]);
      float cey = ts_n * (origin_e[2] * iNN_N[0] - origin_e[0] * iNN_N[2]);
      float cez = ts_n * (origin_e[0] * iNN_N[1] - origin_e[1] * iNN_N[0]);

      float nex = ts_n * iNN_N[0];
      float ney = ts_n * iNN_N[1];
      float nez = ts_n * iNN_N[2];

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
  if (frame_m0->ID > 1){
    Eigen::Vector3f trans_b_m0 = frame_m0->trans_b;
    Eigen::Vector3f trans_e_m0 = frame_m0->trans_e;
    Eigen::Vector3f trans_b_m1 = frame_m1->trans_b;
    Eigen::Vector3f trans_e_m1 = frame_m1->trans_e;

    Eigen::Vector3f diff_traj = trans_b_m0 - trans_e_m1;
    J(3, 3) = J(3, 3) + lambda_location;
    J(4, 4) = J(4, 4) + lambda_location;
    J(5, 5) = J(5, 5) + lambda_location;
    b(3) = b(3) - lambda_location * diff_traj(0);
    b(4) = b(4) - lambda_location * diff_traj(1);
    b(5) = b(5) - lambda_location * diff_traj(2);

    Eigen::Vector3f diff_ego = trans_e_m0 - trans_b_m0 - trans_e_m1 + trans_b_m1;
    J(9, 9)   = J(9, 9)   + lambda_displace;
    J(10, 10) = J(10, 10) + lambda_displace;
    J(11, 11) = J(11, 11) + lambda_displace;
    b(9)  = b(9)  - lambda_displace * diff_ego(0);
    b(10) = b(10) - lambda_displace * diff_ego(1);
    b(11) = b(11) - lambda_displace * diff_ego(2);
  }

  //---------------------------
}
Eigen::Matrix3f SLAM_optim_gn::compute_rotationMatrix(float rx, float ry, float rz){
  Eigen::Matrix3f R;
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
  Eigen::Matrix3f opt_rotat_b = compute_rotationMatrix(X(0), X(1), X(2));
  Eigen::Vector3f opt_trans_b = Eigen::Vector3f(X(3), X(4), X(5));

  Eigen::Matrix3f opt_rotat_e = compute_rotationMatrix(X(6), X(7), X(8));
  Eigen::Vector3f opt_trans_e = Eigen::Vector3f(X(9), X(10), X(11));

  //Update parameters
  frame->rotat_b *= opt_rotat_b;
  frame->trans_b += opt_trans_b;

  frame->rotat_e *= opt_rotat_e;
  frame->trans_e += opt_trans_e;

  //---------------------------
}
void SLAM_optim_gn::update_keypoints(Frame* frame){
  //---------------------------

  //Update keypoints
  Eigen::Quaternionf quat_b = Eigen::Quaternionf(frame->rotat_b);
  Eigen::Quaternionf quat_e = Eigen::Quaternionf(frame->rotat_e);
  Eigen::Vector3f trans_b = frame->trans_b;
  Eigen::Vector3f trans_e = frame->trans_e;

  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3f point_raw = frame->xyz_raw[i];
    Eigen::Vector3f& point = frame->xyz[i];
    float ts_n = frame->ts_n[i];

    Eigen::Matrix3f R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3f t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    point = R * point_raw + t;
  }

  //---------------------------
}
