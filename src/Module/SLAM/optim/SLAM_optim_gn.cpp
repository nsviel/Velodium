
#include "SLAM_optim_gn.h"

#include "SLAM_normal.h"

#include "../src/SLAM.h"

#include "../../../Specific/Function/fct_terminal.h"
#include "../../../Specific/Function/fct_transtypage.h"
#include "../../../Specific/Function/fct_math.h"


//Constructor / Destructor
SLAM_optim_gn::SLAM_optim_gn(SLAM* slam){
  //---------------------------

  this->slam_normal = slam->get_slam_normal();

  //---------------------------
  this->update_configuration();
}
SLAM_optim_gn::~SLAM_optim_gn(){}

//Main function
void SLAM_optim_gn::update_configuration(){
  //---------------------------

  this->iter_max = 5;
  this->nb_thread = 8;
  this->dist_residual_max = 0.5f;
  this->dist_residual_min = 0.01f;
  this->lambda_location_consistency = 0.001;
  this->lambda_constant_velocity =  0.001;

  //---------------------------
}
void SLAM_optim_gn::optim_GN(Frame* frame_m0, Frame* frame_m1){
  // X [0-2]: rotat_b
  // X [3-5]: trans_b
  // X [6-8]: rotat_e
  // X [9-11]: trans_e
  //---------------------------

  for (int iter=0; iter < iter_max; iter++){
    //Initialization
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(12, 12);
    Eigen::VectorXd b = Eigen::VectorXd::Zero(12);

    //Derive residuals
    slam_normal->compute_normal(frame_m0);
    this->compute_derivative(frame_m0);
    this->compute_matrices(frame_m0, J, b);
    this->compute_constraint(frame_m0, frame_m1, J, b);

    //Solve
    X = J.ldlt().solve(b);

    //Update
    this->update_frame(frame_m0, X);
    this->update_keypoints(frame_m0);
  }

  //---------------------------
}

//Sub-function
void SLAM_optim_gn::compute_derivative(Frame* frame){
  vec_u.clear(); vec_u.resize(frame->xyz.size());
  frame->nb_residual = 0;
  frame->nb_residual_false = 0;
  //---------------------------

  //compute residual parameters
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d point = frame->xyz[i];
    Eigen::Vector3d point_raw = frame->xyz_raw[i];
    Eigen::Vector3d normal = frame->N_nn[i];
    Eigen::Vector3d nn = frame->nn[i];
    double ts_n = frame->ts_n[i];
    double a2D = frame->a2D[i];

    //Check for NaN
    if(isnan(a2D) || fct_is_nan(nn) || fct_is_nan(normal)){
      frame->nb_residual_false++;
      continue;
    }

    //Compute point-to-plane distance
    double dist_residual = 0;
    for(int j=0; j<3; j++){
      dist_residual = dist_residual + normal[j] * (point[j] - nn[j]);
    }
    if(abs(dist_residual) > dist_residual_max || abs(dist_residual) < dist_residual_min){
      frame->nb_residual_false++;
      continue;
    }

    //Compute residual
    Eigen::Vector3d N_nn = a2D * a2D * normal;
    double residual = 0;
    for(int j=0; j<3; j++){
      residual = residual + N_nn[j] * (point[j] - nn[j]);
    }
    frame->nb_residual++;

    //Compute parameters
    Eigen::Vector3d origin_b = frame->rotat_b * point_raw;
    Eigen::Vector3d origin_e = frame->rotat_e * point_raw;

    double cx_b = (1 - ts_n) * (origin_b[1] * N_nn[2] - origin_b[2] * N_nn[1]);
    double cy_b = (1 - ts_n) * (origin_b[2] * N_nn[0] - origin_b[0] * N_nn[2]);
    double cz_b = (1 - ts_n) * (origin_b[0] * N_nn[1] - origin_b[1] * N_nn[0]);

    double nx_b = (1 - ts_n) * N_nn[0];
    double ny_b = (1 - ts_n) * N_nn[1];
    double nz_b = (1 - ts_n) * N_nn[2];

    double cx_e = ts_n * (origin_e[1] * N_nn[2] - origin_e[2] * N_nn[1]);
    double cy_e = ts_n * (origin_e[2] * N_nn[0] - origin_e[0] * N_nn[2]);
    double cz_e = ts_n * (origin_e[0] * N_nn[1] - origin_e[1] * N_nn[0]);

    double nx_e = ts_n * N_nn[0];
    double ny_e = ts_n * N_nn[1];
    double nz_e = ts_n * N_nn[2];

    //Store results
    Eigen::VectorXd u = Eigen::VectorXd::Zero(13);
    u << cx_b, cy_b, cz_b, nx_b, ny_b, nz_b, cx_e, cy_e, cz_e, nx_e, ny_e, nz_e, residual;
    vec_u[i] = u;
  }

  //---------------------------
}
void SLAM_optim_gn::compute_matrices(Frame* frame, Eigen::MatrixXd& J, Eigen::VectorXd& b){
  //---------------------------

  //Apply parameters & residuals
  for(int i=0; i<frame->xyz.size(); i++){
    if(vec_u[i].size() != 0){

      for(int j=0; j<12; j++){
        for(int k=0; k<12; k++){
          J(j, k) = J(j, k) + vec_u[i][j] * vec_u[i][k];
        }
        b(j) = b(j) - vec_u[i][j] * vec_u[i][12];
      }

    }
  }

  // fct_normalize equation
  #pragma omp parallel for num_threads(nb_thread)
  for(int i=0; i<12; i++){
    for(int j=0; j<12; j++){
      J(i, j) = J(i, j) / frame->nb_residual;
    }
    b(i) = b(i) / frame->nb_residual;
  }

  //---------------------------
}
void SLAM_optim_gn::compute_constraint(Frame* frame_m0, Frame* frame_m1, Eigen::MatrixXd& J, Eigen::VectorXd& b){
  //---------------------------

  //Add constraints in trajectory
  Eigen::Vector3d trans_b_m0 = frame_m0->trans_b;
  Eigen::Vector3d trans_e_m0 = frame_m0->trans_e;
  Eigen::Vector3d trans_b_m1 = frame_m1->trans_b;
  Eigen::Vector3d trans_e_m1 = frame_m1->trans_e;

  Eigen::Vector3d diff_traj = trans_b_m0 - trans_e_m1;
  J(3, 3) = J(3, 3) + lambda_location_consistency;
  J(4, 4) = J(4, 4) + lambda_location_consistency;
  J(5, 5) = J(5, 5) + lambda_location_consistency;
  b(3) = b(3) - lambda_location_consistency * diff_traj(0);
  b(4) = b(4) - lambda_location_consistency * diff_traj(1);
  b(5) = b(5) - lambda_location_consistency * diff_traj(2);

  Eigen::Vector3d diff_ego = trans_e_m0 - trans_b_m0 - trans_e_m1 + trans_b_m1;
  J(3, 3) = J(3, 3) + lambda_constant_velocity;
  J(4, 4) = J(4, 4) + lambda_constant_velocity;
  J(5, 5) = J(5, 5) + lambda_constant_velocity;
  J(3, 9) = J(3, 9) - lambda_constant_velocity;
  J(4, 10) = J(4, 10) - lambda_constant_velocity;
  J(5, 11) = J(5, 11) - lambda_constant_velocity;
  J(9, 3) = J(9, 3) - lambda_constant_velocity;
  J(10, 4) = J(10, 4) - lambda_constant_velocity;
  J(11, 5) = J(11, 5) - lambda_constant_velocity;
  J(9, 9)   = J(9, 9)   + lambda_constant_velocity;
  J(10, 10) = J(10, 10) + lambda_constant_velocity;
  J(11, 11) = J(11, 11) + lambda_constant_velocity;

  b(3) = b(3) + lambda_constant_velocity * diff_ego(0);
  b(4) = b(4) + lambda_constant_velocity * diff_ego(1);
  b(5) = b(5) + lambda_constant_velocity * diff_ego(2);
  b(9)  = b(9)  - lambda_constant_velocity * diff_ego(0);
  b(10) = b(10) - lambda_constant_velocity * diff_ego(1);
  b(11) = b(11) - lambda_constant_velocity * diff_ego(2);

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

//Update function
void SLAM_optim_gn::update_frame(Frame* frame, Eigen::VectorXd& X){
  //---------------------------

  //Retrieve parameters
  Eigen::Matrix3d rotat_b; // = compute_rotationMatrix(X(0), X(1), X(2));
  rotat_b = Eigen::AngleAxisd(X(0), Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(X(1), Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(X(2), Eigen::Vector3d::UnitZ());
  Eigen::Vector3d trans_b = Eigen::Vector3d(X(3), X(4), X(5));

  Eigen::Matrix3d rotat_e; // = compute_rotationMatrix(X(6), X(7), X(8));
  rotat_e = Eigen::AngleAxisd(X(6), Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(X(7), Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(X(8), Eigen::Vector3d::UnitZ());
  Eigen::Vector3d trans_e = Eigen::Vector3d(X(9), X(10), X(11));

  //Update parameters
  frame->rotat_b = rotat_b * frame->rotat_b;
  frame->trans_b = frame->trans_b + trans_b;

  frame->rotat_e = rotat_e * frame->rotat_e;
  frame->trans_e = frame->trans_e + trans_e;

  frame->opti_score = X.norm();

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
    double ts_n = frame->ts_n[i];

    Eigen::Quaterniond q = quat_b.slerp(ts_n, quat_e);
    q.normalize();
    Eigen::Matrix3d R = q.toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    frame->xyz[i] = R * frame->xyz_raw[i] + t;
  }

  //---------------------------
}
