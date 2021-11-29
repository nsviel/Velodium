#include "SLAM_optim_gn.h"

#include "../../../Specific/fct_display.h"
#include "../../../Specific/fct_transtypage.h"
#include "../../../Specific/fct_maths.h"
#include "../../../Engine/Data/Database.h"

extern struct Database database;


//Constructor / Destructor
SLAM_optim_gn::SLAM_optim_gn(){
  //---------------------------

  normalManager = new SLAM_normal();

  this->iter_max = 1;

  //---------------------------
}
SLAM_optim_gn::~SLAM_optim_gn(){}

void SLAM_optim_gn::optim_GN(Frame* frame, Frame* frame_m1, voxelMap& map){
  //---------------------------

  //SLAM_optim_gn with Traj constraints
  float beta_location_consistency = 0.001;
  float beta_constant_velocity =  0.001;

  float elapsed_search_neighbors = 0.0;
  float elapsed_select_closest_neighbors = 0.0;
  float elapsed_normals = 0.0;
  float elapsed_A_construction = 0.0;
  float elapsed_solve = 0.0;
  float elapsed_update = 0.0;

  for (int iter(0); iter < 2; iter++) {
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(12, 12);
    Eigen::VectorXd b = Eigen::VectorXd::Zero(12);

    int nb_keypoint = 0;
    float total_scalar = 0;
    float mean_scalar = 0;

    //Compute frame normal
    normalManager->compute_frameNormal(frame, map);

    for(int i=0; i<frame->xyz.size(); i++){
      Eigen::Vector3d point = frame->xyz[i];
      Eigen::Vector3d point_raw = frame->xyz_raw[i];
      Eigen::Vector3d normal = frame->Nxyz[i];
      Eigen::Vector3d iNN = frame->NN[i];
      float ts_n = frame->ts_n[i];
      float a2D = frame->a2D[i];

      //say("---");
      //say(iter);
      //say(point);

      float PTP_distance = 0;
      for(int j=0; j<3; j++){
        PTP_distance += normal[j] * (point[j] - iNN[j]);
      }

      if (fabs(PTP_distance) < 0.5) {
        Eigen::Vector3d iNN_N = a2D * a2D * normal;

        float scalar = 0;
        for(int j=0; j<3; j++){
          scalar += iNN_N[j] * (point[j] - iNN[j]);
        }

        total_scalar = total_scalar + scalar * scalar;
        mean_scalar = mean_scalar + fabs(scalar);
        nb_keypoint++;

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
            A(j, k) = A(j, k) + u[j] * u[k];
          }
          b(j) = b(j) - u[j] * scalar;
        }
      }

    }

    if (nb_keypoint < 100) {
        cout << "[CT_ICP]Error : not enough keypoints selected in ct-icp !" << endl;
        cout << "[CT_ICP]Number_of_residuals : " << nb_keypoint << endl;
    }

    // Normalize equation
    for (int i(0); i < 12; i++) {
        for (int j(0); j < 12; j++) {
            A(i, j) = A(i, j) / nb_keypoint;
        }
        b(i) = b(i) / nb_keypoint;
    }

    //Add constraints in trajectory
    if (frame->ID > 1){
      Eigen::Vector3d trans_b = frame->trans_b;
      Eigen::Vector3d trans_e = frame->trans_e;
      Eigen::Vector3d trans_e_m1 = frame_m1->trans_e;

      Eigen::Vector3d diff_traj = trans_b - trans_e_m1;
      A(3, 3) = A(3, 3) + beta_location_consistency;
      A(4, 4) = A(4, 4) + beta_location_consistency;
      A(5, 5) = A(5, 5) + beta_location_consistency;
      b(3) = b(3) - beta_location_consistency * diff_traj(0);
      b(4) = b(4) - beta_location_consistency * diff_traj(1);
      b(5) = b(5) - beta_location_consistency * diff_traj(2);

      Eigen::Vector3d diff_ego = trans_e - trans_b - trans_e_m1 + trans_b;
      A(9, 9) = A(9, 9) + beta_constant_velocity;
      A(10, 10) = A(10, 10) + beta_constant_velocity;
      A(11, 11) = A(11, 11) + beta_constant_velocity;
      b(9) = b(9) - beta_constant_velocity * diff_ego(0);
      b(10) = b(10) - beta_constant_velocity * diff_ego(1);
      b(11) = b(11) - beta_constant_velocity * diff_ego(2);
    }

    //Solve
    Eigen::VectorXd X = A.ldlt().solve(b);


    say("(--------)");
    say(nb_keypoint);
    say(A);say(b);

    float Rx_b = X(0);
    float Ry_b = X(1);
    float Rz_b = X(2);
    Eigen::Matrix3d gn_rotat_b;
    gn_rotat_b(0, 0) = cos(Rz_b) * cos(Ry_b);
    gn_rotat_b(0, 1) = -sin(Rz_b) * cos(Rx_b) + cos(Rz_b) * sin(Ry_b) * sin(Rx_b);
    gn_rotat_b(0, 2) = sin(Rz_b) * sin(Rx_b) + cos(Rz_b) * sin(Ry_b) * cos(Rx_b);
    gn_rotat_b(1, 0) = sin(Rz_b) * cos(Ry_b);
    gn_rotat_b(1, 1) = cos(Rz_b) * cos(Rx_b) + sin(Rz_b) * sin(Ry_b) * sin(Rx_b);
    gn_rotat_b(1, 2) = -cos(Rz_b) * sin(Rx_b) + sin(Rz_b) * sin(Ry_b) * cos(Rx_b);
    gn_rotat_b(2, 0) = -sin(Ry_b);
    gn_rotat_b(2, 1) = cos(Ry_b) * sin(Rx_b);
    gn_rotat_b(2, 2) = cos(Ry_b) * cos(Rx_b);
    Eigen::Vector3d gn_trans_b = Eigen::Vector3d(X(3), X(4), X(5));

    float Rx_e = X(6);
    float Ry_e = X(7);
    float Rz_e = X(8);
    Eigen::Matrix3d gn_rotat_e;
    gn_rotat_e(0, 0) = cos(Rz_e) * cos(Ry_e);
    gn_rotat_e(0, 1) = -sin(Rz_e) * cos(Rx_e) + cos(Rz_e) * sin(Ry_e) * sin(Rx_e);
    gn_rotat_e(0, 2) = sin(Rz_e) * sin(Rx_e) + cos(Rz_e) * sin(Ry_e) * cos(Rx_e);
    gn_rotat_e(1, 0) = sin(Rz_e) * cos(Ry_e);
    gn_rotat_e(1, 1) = cos(Rz_e) * cos(Rx_e) + sin(Rz_e) * sin(Ry_e) * sin(Rx_e);
    gn_rotat_e(1, 2) = -cos(Rz_e) * sin(Rx_e) + sin(Rz_e) * sin(Ry_e) * cos(Rx_e);
    gn_rotat_e(2, 0) = -sin(Ry_e);
    gn_rotat_e(2, 1) = cos(Ry_e) * sin(Rx_e);
    gn_rotat_e(2, 2) = cos(Ry_e) * cos(Rx_e);
    Eigen::Vector3d gn_trans_e = Eigen::Vector3d(X(9), X(10), X(11));

    frame->rotat_b = gn_rotat_b * frame->rotat_b;
    frame->trans_b = gn_trans_b + frame->trans_b;
    frame->rotat_e = gn_rotat_e * frame->rotat_e;
    frame->trans_e = gn_trans_e + frame->trans_e;

    //Update keypoints
    for(int i=0; i<frame->xyz.size(); i++){
      Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
      Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
      Eigen::Vector3d trans_b = frame->trans_b;
      Eigen::Vector3d trans_e = frame->trans_e;
      Eigen::Vector3d point_raw = frame->xyz_raw[i];
      float ts_n = frame->ts_n[i];

      Eigen::Quaterniond q = quat_b.slerp(ts_n, quat_e);
      Eigen::Matrix3d R = q.normalized().toRotationMatrix();
      Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;
      frame->xyz[i] = R * point_raw + t;
    }

  }

  //---------------------------
}

void SLAM_optim_gn::frame_update(Frame* frame){
  //---------------------------

  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  for (int i=0; i<frame->xyz.size(); i++){

    float ts_n = frame->ts_n[i];
    Eigen::Vector3d& point = frame->xyz[i];

    //Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    point = point + trans_e;
  }

  //---------------------------
  frame->xyz_raw = frame->xyz;
}
void SLAM_optim_gn::frame_update_secured(Frame* frame){
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  for (int i=0; i<frame->xyz.size(); i++){

    float ts_n = frame->ts_n[i];
    Eigen::Vector3d& point = frame->xyz[i];

    Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    point = R * point + t;
  }

  //---------------------------
  frame->xyz_raw = frame->xyz;
}
void SLAM_optim_gn::frame_distort(Frame* frame){
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  // Distorts the frame (put all raw_points in the coordinate frame of the pose at the end of the acquisition)
  Eigen::Quaterniond quat_e_inv = quat_e.inverse(); // Rotation of the inverse pose
  Eigen::Vector3d trans_e_inv = -1.0 * (quat_e_inv * trans_e); // Translation of the inverse pose

  for (int i(0); i < frame->xyz.size(); ++i) {

    float ts_n = frame->ts_n[i];
    Eigen::Vector3d point = frame->xyz[i];

    Eigen::Quaterniond quat_n = quat_b.slerp(ts_n, quat_e).normalized();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    // Distort Raw Keypoints
    frame->xyz[i] = quat_e_inv * (quat_n * point + t) + trans_e_inv;

  }

  //---------------------------
}
