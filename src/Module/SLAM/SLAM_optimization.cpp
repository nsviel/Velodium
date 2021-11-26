#include "SLAM_optimization.h"

#include "../../Specific/fct_display.h"
#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"
#include "../../Engine/Data/Database.h"

extern struct Database database;


//Constructor / Destructor
SLAM_optimization::SLAM_optimization(){
  //---------------------------

  slam_normManager = new SLAM_normal();

  this->iter_max = 1;

  //---------------------------
}
SLAM_optimization::~SLAM_optimization(){}

void SLAM_optimization::optim_CERES(Frame* frame, Frame* frame_m1){
  //---------------------------

  //Parameters
  int iter_max = 20;
  int nb_threads = 8;

  //For each subset
  this->frame_distort(frame);
  this->frame_update(frame);

  //ICP SLAM_optimization loop
  for(int iter=0; iter<iter_max; iter++){
    ceres::Problem* ceres_problem = new ceres::Problem();

    cout<<"Iteration "<<iter<<endl;

    //Compute residual vectors for each point
    this->ceres_computePointResidual(frame);

    //Add parameters to optimize
    this->ceres_addParameter(ceres_problem, frame);

    //Add residual of each point
    this->ceres_addPointResidual(ceres_problem, frame);

    //Add some other residual functors
    //this->ceres_addConstraintResidual(ceres_problem, frame, frame_m1, nb_residual);
sayHello();
    //Compute solver
    this->ceres_solve(ceres_problem);
sayHello();
    //Distort for some reason
    this->frame_distort(frame);
    this->frame_update(frame);

    delete ceres_problem;
  }

  //---------------------------
}
void SLAM_optimization::optim_GN(){
  /*
  //SLAM_optimization with Traj constraints
  float ALPHA_C = options.beta_location_consistency; // 0.001;
  float ALPHA_E = options.beta_constant_velocity; // 0.001; //no ego (0.0) is not working

  // For the 50 first frames, visit 2 voxels
  const short nb_voxels_visited = index_frame < options.init_num_frames ? 2 : 1;
  int number_keypoints_used = 0;
  const int kNN = options.min_number_neighbors;

  using AType = Eigen::Matrix<float, 12, 12>;
  using bType = Eigen::Matrix<float, 12, 1>;
  AType A;
  bType b;

  // TODO Remove chronos
  float elapsed_search_neighbors = 0.0;
  float elapsed_select_closest_neighbors = 0.0;
  float elapsed_normals = 0.0;
  float elapsed_A_construction = 0.0;
  float elapsed_solve = 0.0;
  float elapsed_update = 0.0;

  ICPSummary summary;

  int iter_max = index_frame < options.init_num_frames ? 15 : options.num_iters_icp;
  for (int iter(0); iter < iter_max; iter++) {
      A = Eigen::MatrixXd::Zero(12, 12);
      b = Eigen::VectorXd::Zero(12);

      number_keypoints_used = 0;
      float total_scalar = 0;
      float mean_scalar = 0.0;

      for (auto &keypoint: keypoints) {
          auto start = std::chrono::steady_clock::now();
          auto &pt_keypoint = keypoint.pt;

          // Neighborhood search
          vector<Eigen::Vector3d> vector_neighbors = search_neighbors(voxels_map, pt_keypoint,
                                                            nb_voxels_visited, options.size_voxelMap,
                                                            options.max_number_neighbors);
          auto step1 = std::chrono::steady_clock::now();
          std::chrono::duration<float> _elapsed_search_neighbors = step1 - start;
          elapsed_search_neighbors += _elapsed_search_neighbors.count() * 1000.0;


          if (vector_neighbors.size() < kNN) {
              continue;
          }

          auto step2 = std::chrono::steady_clock::now();
          std::chrono::duration<float> _elapsed_neighbors_selection = step2 - step1;
          elapsed_select_closest_neighbors += _elapsed_neighbors_selection.count() * 1000.0;

          // Compute normals from neighbors
          auto neighborhood = compute_neighborhood_distribution(vector_neighbors);
          float planarity_weight = neighborhood.a2D;
          auto &normal = neighborhood.normal;

          if (normal.dot(trajectory[index_frame].trans_b - pt_keypoint) < 0) {
              normal = -1.0 * normal;
          }

          float ts_n = keypoint.ts_n;
          float weight = planarity_weight *
                          planarity_weight; //planarity_weight**2 much better than planarity_weight (planarity_weight**3 is not working)
          Eigen::Vector3d closest_pt_normal = weight * normal;

          Eigen::Vector3d closest_point = vector_neighbors[0];

          float dist_to_plane = normal[0] * (pt_keypoint[0] - closest_point[0]) +
                                 normal[1] * (pt_keypoint[1] - closest_point[1]) +
                                 normal[2] * (pt_keypoint[2] - closest_point[2]);

          auto step3 = std::chrono::steady_clock::now();
          std::chrono::duration<float> _elapsed_normals = step3 - step2;
          elapsed_normals += _elapsed_normals.count() * 1000.0;

          // std::cout << "dist_to_plane : " << dist_to_plane << std::endl;

          if (fabs(dist_to_plane) < options.max_dist_to_plane_ct_icp) {

              float scalar = closest_pt_normal[0] * (pt_keypoint[0] - closest_point[0]) +
                              closest_pt_normal[1] * (pt_keypoint[1] - closest_point[1]) +
                              closest_pt_normal[2] * (pt_keypoint[2] - closest_point[2]);
              total_scalar = total_scalar + scalar * scalar;
              mean_scalar = mean_scalar + fabs(scalar);
              number_keypoints_used++;


              Eigen::Vector3d frame_idx_previous_origin_begin =
                      trajectory[index_frame].rotat_b * keypoint.raw_pt;
              Eigen::Vector3d frame_idx_previous_origin_end =
                      trajectory[index_frame].rotat_e * keypoint.raw_pt;

              float cbx =
                      (1 - ts_n) * (frame_idx_previous_origin_begin[1] * closest_pt_normal[2] -
                                               frame_idx_previous_origin_begin[2] * closest_pt_normal[1]);
              float cby =
                      (1 - ts_n) * (frame_idx_previous_origin_begin[2] * closest_pt_normal[0] -
                                               frame_idx_previous_origin_begin[0] * closest_pt_normal[2]);
              float cbz =
                      (1 - ts_n) * (frame_idx_previous_origin_begin[0] * closest_pt_normal[1] -
                                               frame_idx_previous_origin_begin[1] * closest_pt_normal[0]);

              float nbx = (1 - ts_n) * closest_pt_normal[0];
              float nby = (1 - ts_n) * closest_pt_normal[1];
              float nbz = (1 - ts_n) * closest_pt_normal[2];

              float cex = (ts_n) * (frame_idx_previous_origin_end[1] * closest_pt_normal[2] -
                                                frame_idx_previous_origin_end[2] * closest_pt_normal[1]);
              float cey = (ts_n) * (frame_idx_previous_origin_end[2] * closest_pt_normal[0] -
                                                frame_idx_previous_origin_end[0] * closest_pt_normal[2]);
              float cez = (ts_n) * (frame_idx_previous_origin_end[0] * closest_pt_normal[1] -
                                                frame_idx_previous_origin_end[1] * closest_pt_normal[0]);

              float nex = (ts_n) * closest_pt_normal[0];
              float ney = (ts_n) * closest_pt_normal[1];
              float nez = (ts_n) * closest_pt_normal[2];

              Eigen::VectorXd u(12);
              u << cbx, cby, cbz, nbx, nby, nbz, cex, cey, cez, nex, ney, nez;
              for (int i = 0; i < 12; i++) {
                  for (int j = 0; j < 12; j++) {
                      A(i, j) = A(i, j) + u[i] * u[j];
                  }
                  b(i) = b(i) - u[i] * scalar;
              }


              auto step4 = std::chrono::steady_clock::now();
              std::chrono::duration<float> _elapsed_A = step4 - step3;
              elapsed_search_neighbors += _elapsed_A.count() * 1000.0;
          }
      }


      if (number_keypoints_used < 100) {
          std::stringstream ss_out;
          ss_out << "[CT_ICP]Error : not enough keypoints selected in ct-icp !" << std::endl;
          ss_out << "[CT_ICP]Number_of_residuals : " << number_keypoints_used << std::endl;

          summary.error_log = ss_out.str();
          if (options.debug_print)
              std::cout << summary.error_log;

          summary.success = false;
          return summary;
      }

      auto start = std::chrono::steady_clock::now();


      // Normalize equation
      for (int i(0); i < 12; i++) {
          for (int j(0); j < 12; j++) {
              A(i, j) = A(i, j) / number_keypoints_used;
          }
          b(i) = b(i) / number_keypoints_used;
      }

      //Add constraints in trajectory
      if (index_frame > 1) //no constraints for frame_index == 1
      {
          Eigen::Vector3d diff_traj = trajectory[index_frame].trans_b - trajectory[index_frame - 1].trans_e;
          A(3, 3) = A(3, 3) + ALPHA_C;
          A(4, 4) = A(4, 4) + ALPHA_C;
          A(5, 5) = A(5, 5) + ALPHA_C;
          b(3) = b(3) - ALPHA_C * diff_traj(0);
          b(4) = b(4) - ALPHA_C * diff_traj(1);
          b(5) = b(5) - ALPHA_C * diff_traj(2);

          Eigen::Vector3d diff_ego = trajectory[index_frame].trans_e - trajectory[index_frame].trans_b -
                                     trajectory[index_frame - 1].trans_e + trajectory[index_frame - 1].trans_b;
          A(9, 9) = A(9, 9) + ALPHA_E;
          A(10, 10) = A(10, 10) + ALPHA_E;
          A(11, 11) = A(11, 11) + ALPHA_E;
          b(9) = b(9) - ALPHA_E * diff_ego(0);
          b(10) = b(10) - ALPHA_E * diff_ego(1);
          b(11) = b(11) - ALPHA_E * diff_ego(2);
      }


      //Solve
      Eigen::VectorXd x_bundle = A.ldlt().solve(b);

      float alpha_begin = x_bundle(0);
      float beta_begin = x_bundle(1);
      float gamma_begin = x_bundle(2);
      Eigen::Matrix3d rotation_begin;
      rotation_begin(0, 0) = cos(gamma_begin) * cos(beta_begin);
      rotation_begin(0, 1) =
              -sin(gamma_begin) * cos(alpha_begin) + cos(gamma_begin) * sin(beta_begin) * sin(alpha_begin);
      rotation_begin(0, 2) =
              sin(gamma_begin) * sin(alpha_begin) + cos(gamma_begin) * sin(beta_begin) * cos(alpha_begin);
      rotation_begin(1, 0) = sin(gamma_begin) * cos(beta_begin);
      rotation_begin(1, 1) =
              cos(gamma_begin) * cos(alpha_begin) + sin(gamma_begin) * sin(beta_begin) * sin(alpha_begin);
      rotation_begin(1, 2) =
              -cos(gamma_begin) * sin(alpha_begin) + sin(gamma_begin) * sin(beta_begin) * cos(alpha_begin);
      rotation_begin(2, 0) = -sin(beta_begin);
      rotation_begin(2, 1) = cos(beta_begin) * sin(alpha_begin);
      rotation_begin(2, 2) = cos(beta_begin) * cos(alpha_begin);
      Eigen::Vector3d translation_begin = Eigen::Vector3d(x_bundle(3), x_bundle(4), x_bundle(5));

      float alpha_end = x_bundle(6);
      float beta_end = x_bundle(7);
      float gamma_end = x_bundle(8);
      Eigen::Matrix3d rotation_end;
      rotation_end(0, 0) = cos(gamma_end) * cos(beta_end);
      rotation_end(0, 1) = -sin(gamma_end) * cos(alpha_end) + cos(gamma_end) * sin(beta_end) * sin(alpha_end);
      rotation_end(0, 2) = sin(gamma_end) * sin(alpha_end) + cos(gamma_end) * sin(beta_end) * cos(alpha_end);
      rotation_end(1, 0) = sin(gamma_end) * cos(beta_end);
      rotation_end(1, 1) = cos(gamma_end) * cos(alpha_end) + sin(gamma_end) * sin(beta_end) * sin(alpha_end);
      rotation_end(1, 2) = -cos(gamma_end) * sin(alpha_end) + sin(gamma_end) * sin(beta_end) * cos(alpha_end);
      rotation_end(2, 0) = -sin(beta_end);
      rotation_end(2, 1) = cos(beta_end) * sin(alpha_end);
      rotation_end(2, 2) = cos(beta_end) * cos(alpha_end);
      Eigen::Vector3d translation_end = Eigen::Vector3d(x_bundle(9), x_bundle(10), x_bundle(11));

      trajectory[index_frame].rotat_b = rotation_begin * trajectory[index_frame].rotat_b;
      trajectory[index_frame].trans_b = trajectory[index_frame].trans_b + translation_begin;
      trajectory[index_frame].rotat_e = rotation_end * trajectory[index_frame].rotat_e;
      trajectory[index_frame].trans_e = trajectory[index_frame].trans_e + translation_end;

      auto solve_step = std::chrono::steady_clock::now();
      std::chrono::duration<float> _elapsed_solve = solve_step - start;
      elapsed_solve += _elapsed_solve.count() * 1000.0;


      //Update keypoints
      for (auto &keypoint: keypoints) {
          Eigen::Quaterniond q_begin = Eigen::Quaterniond(trajectory[index_frame].rotat_b);
          Eigen::Quaterniond q_end = Eigen::Quaterniond(trajectory[index_frame].rotat_e);
          Eigen::Vector3d t_begin = trajectory[index_frame].trans_b;
          Eigen::Vector3d t_end = trajectory[index_frame].trans_e;
          float ts_n = keypoint.ts_n;
          Eigen::Quaterniond q = q_begin.slerp(ts_n, q_end);
          q.normalize();
          Eigen::Matrix3d R = q.toRotationMatrix();
          Eigen::Vector3d t = (1.0 - ts_n) * t_begin + ts_n * t_end;
          keypoint.pt = R * keypoint.raw_pt + t;
      }
      auto update_step = std::chrono::steady_clock::now();
      std::chrono::duration<float> _elapsed_update = update_step - solve_step;
      elapsed_update += _elapsed_update.count() * 1000.0;


      if ((index_frame > 1) && (x_bundle.norm() < options.threshold_orientation_norm)) {
          summary.success = true;
          summary.num_residuals_used = number_keypoints_used;

          return summary;
      }
  }

  if (options.debug_print) {
      std::cout << "Elapsed Normals: " << elapsed_normals << std::endl;
      std::cout << "Elapsed Search Neighbors: " << elapsed_search_neighbors << std::endl;
      std::cout << "Elapsed A Construction: " << elapsed_A_construction << std::endl;
      std::cout << "Elapsed Select closest: " << elapsed_select_closest_neighbors << std::endl;
      std::cout << "Elapsed Solve: " << elapsed_solve << std::endl;
      std::cout << "Elapsed Solve: " << elapsed_update << std::endl;
      std::cout << "Number iterations CT-ICP : " << options.num_iters_icp << std::endl;
  }
  summary.success = true;
  summary.num_residuals_used = number_keypoints_used;

  return summary;*/
}
void SLAM_optimization::optim_test(Frame* frame, Frame* frame_m1, voxelMap& map){
  //---------------------------

  for(int iter=0; iter<iter_max; iter++){
    //Set the frame at position
    this->frame_update(frame);

    //Compute frame normal
    slam_normManager->compute_frameNormal(frame, map);

    //Parameter block
    ceres::Problem* ceres_problem = new ceres::Problem();
    Eigen::Vector3d trans_b = frame->trans_b;
    Eigen::Vector3d trans_e = frame->trans_e;
    double* trans_b_ptr = &trans_b.x();
    double* trans_e_ptr = &trans_e.x();
    ceres_problem->AddParameterBlock(trans_b_ptr, 3);
    ceres_problem->AddParameterBlock(trans_e_ptr, 3);

    //Compute residuals
    vector_residual.clear();
    for(int i=0; i<frame->xyz.size(); i++){
      Eigen::Vector3d point = frame->xyz[i];
      Eigen::Vector3d normal = frame->Nxyz[i];
      Eigen::Vector3d iNN = frame->NN[i];
      float ts_n = frame->ts_n[i];

      CTPTPF* cost_functor = new CTPTPF(iNN, point, normal, ts_n);
      CTPTPR* residual = new CTPTPR(cost_functor);

      vector_residual.push_back(residual);
    }

    //Add residuals
    for(int i=0; i<vector_residual.size(); i++){
      CTPTPR* residual = vector_residual[i];
      if(i < 600){
        ceres_problem->AddResidualBlock(residual, nullptr, trans_b_ptr, trans_e_ptr);
      }
    }

    //Compute solver
    ceres::Solver::Summary summary;
    ceres::Solver::Options ceres_options;
    ceres_options.max_num_iterations = 20;
    ceres_options.num_threads = 4;
    ceres_options.trust_region_strategy_type = ceres::TrustRegionStrategyType::LEVENBERG_MARQUARDT;
    ceres::Solve(ceres_options, ceres_problem, &summary);

    frame->trans_b = trans_b;
    frame->trans_e = trans_e;

    say(trans_b);
    say(trans_e);
    say(frame->ID);

    say(summary.BriefReport());
  }

  //Update with final optimization results
  this->frame_update(frame);

  //---------------------------
}
void SLAM_optimization::optim_test_secured(Frame* frame, Frame* frame_m1, voxelMap& map){
  //---------------------------

  for(int iter=0; iter<iter_max; iter++){
    //Set the frame at position
    this->frame_update(frame);

    //Compute frame normal
    slam_normManager->compute_frameNormal(frame, map);

    //Parameter block
    ceres::Problem* ceres_problem = new ceres::Problem();
    Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
    Eigen::Vector3d trans_b = frame->trans_b;
    double* quat_b_ptr = &quat_b.x();
    double* trans_b_ptr = &trans_b.x();

    Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
    Eigen::Vector3d trans_e = frame->trans_e;
    double* quat_e_ptr = &quat_e.x();
    double* trans_e_ptr = &trans_e.x();

    ceres::EigenQuaternionParameterization parameterization;
    ceres_problem->AddParameterBlock(quat_b_ptr, 4, &parameterization);
    ceres_problem->AddParameterBlock(trans_b_ptr, 3);
    ceres_problem->AddParameterBlock(quat_e_ptr, 4, &parameterization);
    ceres_problem->AddParameterBlock(trans_e_ptr, 3);

    //Compute residuals
    vector_residual.clear();
    for(int i=0; i<frame->xyz.size(); i++){
      Eigen::Vector3d point = frame->xyz[i];
      Eigen::Vector3d normal = frame->Nxyz[i];
      Eigen::Vector3d iNN = frame->NN[i];
      float a2D = frame->a2D[i];
      float ts_n = frame->ts_n[i];



      CTPTPF* cost_functor = new CTPTPF(iNN, point, normal, ts_n);
      CTPTPR* residual = new CTPTPR(cost_functor);

      vector_residual.push_back(residual);
    }

    //Add residuals
    for(int i=0; i<vector_residual.size(); i++){
      CTPTPR* residual = vector_residual[i];
      if(i < 600){
        ceres_problem->AddResidualBlock(residual, nullptr, quat_b_ptr, trans_b_ptr, quat_e_ptr, trans_e_ptr);
      }
    }

    //ADDOTHERRESidual
    /*
    float beta_location_consistency = 0.001;
    float beta_constant_velocity = 0.01;
    float beta_orientation_consistency = 0;
    float beta_small_velocity = 0.01;

    Eigen::Vector3d previous_velocity = frame_m1->trans_e - frame_m1->trans_b;
    Eigen::Quaterniond previous_orientation = Eigen::Quaterniond(frame_m1->rotat_e);
    ceres::CostFunction* cost_function_LCF = new ceres::AutoDiffCostFunction<LCF,LCF::NumResiduals(), 3>(new LCF(frame_m1->trans_e, sqrt(nb_residual* beta_location_consistency)));
    ceres_problem->AddResidualBlock(cost_function_LCF, nullptr, &trans_b.x());
    ceres::CostFunction* cost_function_CVF = new ceres::AutoDiffCostFunction<CVF,CVF::NumResiduals(), 3, 3>(new CVF(previous_velocity, sqrt(nb_residual * beta_constant_velocity)));
    ceres_problem->AddResidualBlock(cost_function_CVF, nullptr, &trans_b.x(), &trans_e.x());
    ceres::CostFunction* cost_function_SVF = new ceres::AutoDiffCostFunction<SVF,SVF::NumResiduals(), 3, 3>(new SVF(sqrt(nb_residual * beta_small_velocity)));
    ceres_problem->AddResidualBlock(cost_function_SVF, nullptr, &trans_b.x(), &trans_e.x());
    ceres::CostFunction* cost_function_OCF = new ceres::AutoDiffCostFunction<OCF,OCF::NumResiduals(), 4>(new OCF(previous_orientation,sqrt(nb_residual * beta_orientation_consistency)));
    ceres_problem->AddResidualBlock(cost_function_OCF, nullptr, &quat_b.x());
    */

    //Compute solver
    ceres::Solver::Summary summary;
    ceres::Solver::Options ceres_options;
    ceres_options.max_num_iterations = 20;
    ceres_options.num_threads = 4;
    ceres_options.trust_region_strategy_type = ceres::TrustRegionStrategyType::LEVENBERG_MARQUARDT;
    ceres::Solve(ceres_options, ceres_problem, &summary);

    frame->trans_b = trans_b;
    frame->trans_e = trans_e;

    say(trans_b);
    say(trans_e);
    say(frame->ID);

    say(summary.BriefReport());
  }

  //Update with final optimization results
  this->frame_update(frame);

  //---------------------------
}

void SLAM_optimization::frame_update(Frame* frame){
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
void SLAM_optimization::frame_update_secured(Frame* frame){
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
void SLAM_optimization::frame_distort(Frame* frame){
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

void SLAM_optimization::ceres_computePointResidual(Frame* frame){
  //---------------------------

  //Clear variables
  vector_residual.clear();
  this->nb_residual = 0;

  //#pragma omp parallel for num_threads(nb_threads)
  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d point = frame->xyz[i];
    Eigen::Vector3d normal = frame->Nxyz[i];
    Eigen::Vector3d iNN = frame->NN[i];
    float a2D = frame->a2D[i];
    float ts_n = frame->ts_n[i];

    //Parameters
    float lambda_w = 0.8;
    float lambda_iNN = 0.2;
    float PTP_distance_max = 0.5;
    int kNN_max = 20;

    float PTP_distance = std::abs((point - iNN).transpose() * normal);

    if (PTP_distance < PTP_distance_max) {
      float weight = lambda_w * pow(a2D, 2) + lambda_iNN * exp(-(iNN - point).norm() / (PTP_distance_max * kNN_max));

      /*CTPTPF* cost_functor = new CTPTPF(iNN, point, normal, ts_n, weight);
      CTPTPR* residual = new CTPTPR(cost_functor);

      vector_residual.push_back(residual);*/
      nb_residual++;
    }
  }

  //---------------------------
}
void SLAM_optimization::ceres_addParameter(ceres::Problem* ceres_problem, Frame* frame){
  ceres::EigenQuaternionParameterization parameterization;
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  quat_b_ptr = &quat_b.x();
  quat_e_ptr = &quat_e.x();
  trans_b_ptr = &trans_b.x();
  trans_e_ptr = &trans_e.x();

  //CT_POINT_TO_PLANE
  ceres_problem->AddParameterBlock(quat_b_ptr, 4, &parameterization);
  ceres_problem->AddParameterBlock(quat_e_ptr, 4, &parameterization);
  ceres_problem->AddParameterBlock(trans_b_ptr, 3);
  ceres_problem->AddParameterBlock(trans_e_ptr, 3);

  //---------------------------
}
void SLAM_optimization::ceres_addPointResidual(ceres::Problem* ceres_problem, Frame* frame){
  ceres::LossFunction* loss_function = new ceres::CauchyLoss(0.2);
  int max_residual = 600;
  //---------------------------

  for(int i=0; i<vector_residual.size(); i++){
    CTPTPR* residual = vector_residual[i];

    if(i < max_residual){

      ceres_problem->AddResidualBlock(residual, loss_function, quat_b_ptr, trans_b_ptr, quat_e_ptr, trans_e_ptr);

    }
  }

  //---------------------------
}
void SLAM_optimization::ceres_addConstraintResidual(ceres::Problem* ceres_problem, Frame* frame, Frame* frame_m1, int number_of_residuals){
  //---------------------------

  float beta_location_consistency = 0.001;
  float beta_constant_velocity = 0.01;
  float beta_orientation_consistency = 0;
  float beta_small_velocity = 0.01;

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  Eigen::Vector3d previous_velocity = frame_m1->trans_e - frame_m1->trans_b;
  Eigen::Quaterniond previous_orientation = Eigen::Quaterniond(frame_m1->rotat_e);

  // Location Consistency Functor
  ceres::CostFunction* cost_function_LCF = new ceres::AutoDiffCostFunction<LCF,LCF::NumResiduals(), 3>(new LCF(frame_m1->trans_e, sqrt(number_of_residuals* beta_location_consistency)));
  ceres_problem->AddResidualBlock(cost_function_LCF, nullptr, &trans_b.x());

  // Constant Velocity Functor
  ceres::CostFunction* cost_function_CVF = new ceres::AutoDiffCostFunction<CVF,CVF::NumResiduals(), 3, 3>(new CVF(previous_velocity, sqrt(number_of_residuals * beta_constant_velocity)));
  ceres_problem->AddResidualBlock(cost_function_CVF, nullptr, &trans_b.x(), &trans_e.x());

  // Small Velocity Functor
  ceres::CostFunction* cost_function_SVF = new ceres::AutoDiffCostFunction<SVF,SVF::NumResiduals(), 3, 3>(new SVF(sqrt(number_of_residuals * beta_small_velocity)));
  ceres_problem->AddResidualBlock(cost_function_SVF, nullptr, &trans_b.x(), &trans_e.x());

  // Orientation Consistency Functor
  ceres::CostFunction* cost_function_OCF = new ceres::AutoDiffCostFunction<OCF,OCF::NumResiduals(), 4>(new OCF(previous_orientation,sqrt(number_of_residuals * beta_orientation_consistency)));
  ceres_problem->AddResidualBlock(cost_function_OCF, nullptr, &quat_b.x());

  //---------------------------
}
void SLAM_optimization::ceres_solve(ceres::Problem* ceres_problem){
  //---------------------------

  ceres::Solver::Summary summary;
  ceres::Solver::Options ceres_options;

  //Ceres parameters
  ceres_options.max_num_iterations = 20;
  ceres_options.num_threads = 4;
  ceres_options.trust_region_strategy_type = ceres::TrustRegionStrategyType::LEVENBERG_MARQUARDT;

  ceres::Solve(ceres_options, ceres_problem, &summary);

  //---------------------------
}
