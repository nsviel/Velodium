#ifdef USE_CERES
#include "SLAM_optim_ceres.h"

#include "../../../Specific/fct_terminal.h"
#include "../../../Specific/fct_transtypage.h"
#include "../../../Specific/fct_math.h"


//Constructor / Destructor
SLAM_optim_ceres::SLAM_optim_ceres(SLAM_normal* normal){
  this->normalManager = normal;
  //---------------------------

  this->iter_max = 1;

  //---------------------------
}
SLAM_optim_ceres::~SLAM_optim_ceres(){}

void SLAM_optim_ceres::optim_CERES(Frame* frame, Frame* frame_m1){
  //---------------------------

  //Parameters
  int iter_max = 20;
  int nb_threads = 8;

  //For each subset
  this->frame_distort(frame);
  this->frame_update(frame);

  //ICP SLAM_optim_ceres loop
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

    //Compute solver
    this->ceres_solve(ceres_problem);

    //Distort for some reason
    this->frame_distort(frame);
    this->frame_update(frame);

    delete ceres_problem;
  }

  //---------------------------
}
void SLAM_optim_ceres::optim_test(Frame* frame, Frame* frame_m1, voxelMap* map){
  //---------------------------

  for(int iter=0; iter<iter_max; iter++){
    //Set the frame at position
    this->frame_update(frame);

    //Compute frame normal
    normalManager->compute_normal(frame, map);

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
      Eigen::Vector3d normal = frame->N_nn[i];
      Eigen::Vector3d iNN = frame->nn[i];
      double ts_n = frame->ts_n[i];

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
void SLAM_optim_ceres::optim_test_secured(Frame* frame, Frame* frame_m1, voxelMap* map){
  //---------------------------

  for(int iter=0; iter<iter_max; iter++){
    //Set the frame at position
    this->frame_update(frame);

    //Compute frame normal
    normalManager->compute_normal(frame, map);

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
      Eigen::Vector3d normal = frame->N_nn[i];
      Eigen::Vector3d iNN = frame->nn[i];
      double a2D = frame->a2D[i];
      double ts_n = frame->ts_n[i];



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
    double beta_location_consistency = 0.001;
    double beta_constant_velocity = 0.01;
    double beta_orientation_consistency = 0;
    double beta_small_velocity = 0.01;

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

void SLAM_optim_ceres::frame_update(Frame* frame){
  //---------------------------

  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  for (int i=0; i<frame->xyz.size(); i++){

    double ts_n = frame->ts_n[i];
    Eigen::Vector3d& point = frame->xyz[i];

    //Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    point = point + trans_e;
  }

  //---------------------------
  frame->xyz_raw = frame->xyz;
}
void SLAM_optim_ceres::frame_update_secured(Frame* frame){
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  for (int i=0; i<frame->xyz.size(); i++){

    double ts_n = frame->ts_n[i];
    Eigen::Vector3d& point = frame->xyz[i];

    Eigen::Matrix3d R = quat_b.slerp(ts_n, quat_e).normalized().toRotationMatrix();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    point = R * point + t;
  }

  //---------------------------
  frame->xyz_raw = frame->xyz;
}
void SLAM_optim_ceres::frame_distort(Frame* frame){
  //---------------------------

  Eigen::Quaterniond quat_b = Eigen::Quaterniond(frame->rotat_b);
  Eigen::Quaterniond quat_e = Eigen::Quaterniond(frame->rotat_e);
  Eigen::Vector3d trans_b = frame->trans_b;
  Eigen::Vector3d trans_e = frame->trans_e;

  // Distorts the frame (put all raw_points in the coordinate frame of the pose at the end of the acquisition)
  Eigen::Quaterniond quat_e_inv = quat_e.inverse(); // Rotation of the inverse pose
  Eigen::Vector3d trans_e_inv = -1.0 * (quat_e_inv * trans_e); // Translation of the inverse pose

  for (int i(0); i < frame->xyz.size(); ++i) {

    double ts_n = frame->ts_n[i];
    Eigen::Vector3d point = frame->xyz[i];

    Eigen::Quaterniond quat_n = quat_b.slerp(ts_n, quat_e).normalized();
    Eigen::Vector3d t = (1.0 - ts_n) * trans_b + ts_n * trans_e;

    // Distort Raw Keypoints
    frame->xyz[i] = quat_e_inv * (quat_n * point + t) + trans_e_inv;

  }

  //---------------------------
}

void SLAM_optim_ceres::ceres_computePointResidual(Frame* frame){
  //---------------------------

  //Clear variables
  vector_residual.clear();
  this->nb_residual = 0;

  //#pragma omp parallel for num_threads(nb_threads)
  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3d point = frame->xyz[i];
    Eigen::Vector3d normal = frame->N_nn[i];
    Eigen::Vector3d iNN = frame->nn[i];
    double a2D = frame->a2D[i];
    double ts_n = frame->ts_n[i];

    //Parameters
    double lambda_w = 0.8;
    double lambda_iNN = 0.2;
    double PTP_distance_max = 0.5;
    int kNN_max = 20;

    double PTP_distance = std::abs((point - iNN).transpose() * normal);

    if (PTP_distance < PTP_distance_max) {
      double weight = lambda_w * pow(a2D, 2) + lambda_iNN * exp(-(iNN - point).norm() / (PTP_distance_max * kNN_max));

      /*CTPTPF* cost_functor = new CTPTPF(iNN, point, normal, ts_n, weight);
      CTPTPR* residual = new CTPTPR(cost_functor);

      vector_residual.push_back(residual);*/
      nb_residual++;
    }
  }

  //---------------------------
}
void SLAM_optim_ceres::ceres_addParameter(ceres::Problem* ceres_problem, Frame* frame){
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
void SLAM_optim_ceres::ceres_addPointResidual(ceres::Problem* ceres_problem, Frame* frame){
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
void SLAM_optim_ceres::ceres_addConstraintResidual(ceres::Problem* ceres_problem, Frame* frame, Frame* frame_m1, int number_of_residuals){
  //---------------------------

  double beta_location_consistency = 0.001;
  double beta_constant_velocity = 0.01;
  double beta_orientation_consistency = 0;
  double beta_small_velocity = 0.01;

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
void SLAM_optim_ceres::ceres_solve(ceres::Problem* ceres_problem){
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


#endif
