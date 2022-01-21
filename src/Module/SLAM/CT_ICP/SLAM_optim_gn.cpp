#include "SLAM_optim_gn.h"

#include "../../../Specific/fct_display.h"
#include "../../../Specific/fct_transtypage.h"
#include "../../../Specific/fct_maths.h"
#include "../../../Engine/Data/Database.h"

extern struct Database database;


//Constructor / Destructor
SLAM_optim_gn::SLAM_optim_gn(SLAM_normal* normal){
  this->normalManager = normal;
  //---------------------------

  this->iter_max = 5;
  this->nb_thread = 8;
  this->nb_residual_min = 100;
  this->PTP_distance_max = 0.5f;
  this->lambda_location = 0.001;
  this->lambda_displacement =  0.001;

  //---------------------------
}
SLAM_optim_gn::~SLAM_optim_gn(){}

//Main functions
void SLAM_optim_gn::optim_GN(Frame* frame, Frame* frame_m1, voxelMap* map){
  X = Eigen::VectorXd(0);
  //---------------------------

  for (int iter=0; iter < iter_max; iter++) {
    //Initialization
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(12, 12);
    Eigen::VectorXd b = Eigen::VectorXd::Zero(12);

    //Compute frame normal
    normalManager->compute_frameNormal(frame, map);
    this->compute_residuals(frame, J, b);
    this->compute_constraints(frame, frame_m1, J, b);

    //Solve
    X = J.ldlt().solve(b);

    //Update
    this->update_frame(frame, X);
    this->update_keypoints(frame);
  }

  //---------------------------
}

//Subfunctions
void SLAM_optim_gn::update_frame(Frame* frame, Eigen::VectorXd& X){
  bool sucess = true;
  //---------------------------

  //CHeck for good optimization
  if (nb_residual < nb_residual_min) {
    sucess = false;
  }

  //Retrieve parameters
  if(sucess){
    Eigen::Matrix3d gn_rotat_b = compute_rotationMatrix(X(0), X(1), X(2));
    Eigen::Vector3d gn_trans_b = Eigen::Vector3d(X(3), X(4), X(5));

    Eigen::Matrix3d gn_rotat_e = compute_rotationMatrix(X(6), X(7), X(8));
    Eigen::Vector3d gn_trans_e = Eigen::Vector3d(X(9), X(10), X(11));

    //Update parameters
    frame->rotat_b = gn_rotat_b * frame->rotat_b;
    frame->trans_b = gn_trans_b + frame->trans_b;

    frame->rotat_e = gn_rotat_e * frame->rotat_e;
    frame->trans_e = gn_trans_e + frame->trans_e;
  }

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
    float ts_n = frame->ts_n[i];

    Eigen::Quaterniond q = quat_b.slerp(ts_n, quat_e);
    Eigen::Matrix3d R = q.normalized().toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    frame->xyz[i] = R * point_raw + t;
  }

  //---------------------------
}
void SLAM_optim_gn::compute_constraints(Frame* frame, Frame* frame_m1, Eigen::MatrixXd& J, Eigen::VectorXd& b){
  //---------------------------

  //Add constraints in trajectory
  if (frame->ID > 1){
    Eigen::Vector3d trans_b = frame->trans_b;
    Eigen::Vector3d trans_e = frame->trans_e;
    Eigen::Vector3d trans_b_m1 = frame_m1->trans_b;
    Eigen::Vector3d trans_e_m1 = frame_m1->trans_e;

    Eigen::Vector3d diff_traj = trans_b - trans_e_m1;
    J(3, 3) = J(3, 3) + lambda_location;
    J(4, 4) = J(4, 4) + lambda_location;
    J(5, 5) = J(5, 5) + lambda_location;
    b(3) = b(3) - lambda_location * diff_traj(0);
    b(4) = b(4) - lambda_location * diff_traj(1);
    b(5) = b(5) - lambda_location * diff_traj(2);

    Eigen::Vector3d diff_ego = trans_e - trans_b - trans_e_m1 + trans_b_m1;
    J(9, 9) = J(9, 9) + lambda_displacement;
    J(10, 10) = J(10, 10) + lambda_displacement;
    J(11, 11) = J(11, 11) + lambda_displacement;
    b(9) = b(9) - lambda_displacement * diff_ego(0);
    b(10) = b(10) - lambda_displacement * diff_ego(1);
    b(11) = b(11) - lambda_displacement * diff_ego(2);
  }

  //---------------------------
}
void SLAM_optim_gn::compute_residuals(Frame* frame, Eigen::MatrixXd& J, Eigen::VectorXd& b){
  nb_residual = 0;
  //---------------------------

  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d point = frame->xyz[i];
    Eigen::Vector3d point_raw = frame->xyz_raw[i];
    Eigen::Vector3d normal = frame->Nptp[i];
    Eigen::Vector3d iNN = frame->NN[i];
    float ts_n = frame->ts_n[i];
    float a2D = frame->a2D[i];

    float PTP_distance = 0;
    for(int j=0; j<3; j++){
      PTP_distance += normal[j] * (point[j] - iNN[j]);
    }

    if (abs(PTP_distance) < PTP_distance_max && isnan(a2D) == false) {
      //Compute normal of the iNN point
      Eigen::Vector3d iNN_N = a2D * a2D * normal;

      float residual = 0;
      for(int j=0; j<3; j++){
        residual += (point[j] - iNN[j]) * iNN_N[j];
      }

      nb_residual++;

      Eigen::Matrix3d rotat_b = frame->rotat_b;
      Eigen::Matrix3d rotat_e = frame->rotat_e;

      Eigen::Vector3d origin_b = rotat_b * point_raw;
      Eigen::Vector3d origin_e = rotat_e * point_raw;

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

      Eigen::VectorXd u(12);
      u << cbx, cby, cbz, nbx, nby, nbz, cex, cey, cez, nex, ney, nez;
      for (int j = 0; j < 12; j++) {
        for (int k = 0; k < 12; k++) {
          J(j, k) = J(j, k) + u[j] * u[k];
        }
        b(j) = b(j) - u[j] * residual;
      }
    }
  }

  if (nb_residual < nb_residual_min) {
    cout << "[CT_ICP]Error : not enough keypoints selected in ct-icp !" << endl;
    cout << "[CT_ICP]Number_of_residuals : " << nb_residual << endl;
    return;
  }

  // Normalize equation
  #pragma omp parallel for num_threads(nb_thread)
  for (int i=0; i < 12; i++) {
      for (int j=0; j < 12; j++) {
          J(i, j) = J(i, j) / nb_residual;
      }
      b(i) = b(i) / nb_residual;
  }

  //---------------------------
}
Eigen::Matrix3d SLAM_optim_gn::compute_rotationMatrix(float Rx, float Ry, float Rz){
  Eigen::Matrix3d rotat;
  //---------------------------

  rotat(0, 0) = cos(Rz) * cos(Ry);
  rotat(0, 1) = -sin(Rz) * cos(Rx) + cos(Rz) * sin(Ry) * sin(Rx);
  rotat(0, 2) = sin(Rz) * sin(Rx) + cos(Rz) * sin(Ry) * cos(Rx);
  rotat(1, 0) = sin(Rz) * cos(Ry);
  rotat(1, 1) = cos(Rz) * cos(Rx) + sin(Rz) * sin(Ry) * sin(Rx);
  rotat(1, 2) = -cos(Rz) * sin(Rx) + sin(Rz) * sin(Ry) * cos(Rx);
  rotat(2, 0) = -sin(Ry);
  rotat(2, 1) = cos(Ry) * sin(Rx);
  rotat(2, 2) = cos(Ry) * cos(Rx);

  //---------------------------
  return rotat;
}
