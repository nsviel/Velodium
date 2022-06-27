#include "SLAM_parameter.h"

#include "SLAM_normal.h"
#include "SLAM_optim.h"
#include "SLAM_optim_gn.h"
#include "SLAM_assessment.h"
#include "SLAM_map.h"

#include "../Slam.h"


//Constructor / Destructor
SLAM_parameter::SLAM_parameter(Slam* slam){
  //---------------------------

  this->slamManager = slam;
  this->normalManager = slam->get_slam_normal();
  this->optimManager = slam->get_slam_optim();
  this->gnManager = optimManager->get_optim_gn();
  this->assessManager = slam->get_assessManager();
  this->mapManager = slam->get_slam_map();

  this->predefined_conf = 0;

  //---------------------------
}
SLAM_parameter::~SLAM_parameter(){}

//Configuration functions
void SLAM_parameter::make_config(int conf){
  //---------------------------

  switch(conf){
    case 0:{ //Default
      this->make_config_default();
      break;
    }
    case 1:{ //VLP64
      this->make_config_0();
      break;
    }
    case 2:{ //VLP16
      this->make_config_1();
      break;
    }
    case 3:{ //HDL32
      this->make_config_2();
      break;
    }
  }

  //---------------------------
  this->predefined_conf = conf;
}
void SLAM_parameter::make_config(string conf){
  //---------------------------

  if(conf == "default"){
    this->make_config_default();
    this->predefined_conf = 0;
  }
  else if(conf == "velodyne_vlp64"){
    this->make_config_0();
    this->predefined_conf = 1;
  }
  else if(conf == "velodyne_vlp16" || conf == "scala"){
    this->make_config_1();
    this->predefined_conf = 2;
  }
  else if(conf == "velodyne_hdl32"){
    this->make_config_2();
    this->predefined_conf = 3;
  }

  //---------------------------
}
void SLAM_parameter::make_config_default(){
  //velodyne_vlp64
  //---------------------------

  //Slam
  bool* solver_gn = optimManager->get_solver_gn();
  bool* solver_ceres = optimManager->get_solver_ceres();
  double* thres_ego_trans = assessManager->get_thres_ego_trans();
  double* thres_ego_rotat = assessManager->get_thres_ego_rotat();
  double* thres_pose_trans = assessManager->get_thres_pose_trans();
  double* thres_pose_rotat = assessManager->get_thres_pose_rotat();
  double* thres_optimMinNorm = assessManager->get_thres_optimMinNorm();
  int* nb_residual_min = assessManager->get_nb_residual_min();
  int* nb_rlt_previous_mean = assessManager->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = assessManager->get_nb_rlt_previous_pose();

  //Normal
  double* size_voxelMap = normalManager->get_knn_voxel_width();
  int* max_number_neighbors = normalManager->get_knn_max_nn();
  int* voxel_searchSize = normalManager->get_knn_voxel_search();

  //Optim gn
  double*PTP_distance_max = gnManager->get_dist_residual_max();
  int* iter_max = gnManager->get_iter_max();

  //Local map
  double* map_voxel_size = mapManager->get_map_voxel_size();
  double* min_root_distance = mapManager->get_min_root_distance();
  double* max_root_distance = mapManager->get_max_root_distance();
  double* max_voxel_distance = mapManager->get_max_voxel_distance();
  double* min_voxel_distance = mapManager->get_min_voxel_distance();
  double* grid_voxel_size = mapManager->get_grid_voxel_size();
  int* map_voxel_capacity = mapManager->get_map_voxel_capacity();

  //Slam
  *solver_gn = true;
  *solver_ceres = false;
  *thres_ego_trans = 2.0f;
  *thres_ego_rotat = 15.0f;
  *thres_pose_trans = 4.0f;
  *thres_pose_rotat = 15.0f;
  *thres_optimMinNorm = 0.4f;
  *nb_residual_min = 100;
  *nb_rlt_previous_mean = 10;
  *nb_rlt_previous_pose = 4;

  //Normal
  *size_voxelMap = 1.0f;
  *max_number_neighbors = 20;
  *voxel_searchSize = 1;

  //Optim gn
  *PTP_distance_max = 0.3f;
  *iter_max = 5;

  //Local map
  *map_voxel_size = 1.0f;
  *min_root_distance = 5.0f;
  *max_root_distance = 100.0f;
  *max_voxel_distance = 150.0f;
  *min_voxel_distance = 0.1f;
  *grid_voxel_size = 1.0f;
  *map_voxel_capacity = 20;

  //Specific
  this->set_nb_thread(1);

  //---------------------------
}
void SLAM_parameter::make_config_0(){
  //velodyne_vlp64
  //---------------------------

  //Slam
  bool* solver_gn = optimManager->get_solver_gn();
  bool* solver_ceres = optimManager->get_solver_ceres();
  double* thres_ego_trans = assessManager->get_thres_ego_trans();
  double* thres_ego_rotat = assessManager->get_thres_ego_rotat();
  double* thres_pose_trans = assessManager->get_thres_pose_trans();
  double* thres_pose_rotat = assessManager->get_thres_pose_rotat();
  double* thres_optimMinNorm = assessManager->get_thres_optimMinNorm();
  int* nb_residual_min = assessManager->get_nb_residual_min();
  int* nb_rlt_previous_mean = assessManager->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = assessManager->get_nb_rlt_previous_pose();

  //Normal
  double* size_voxelMap = normalManager->get_knn_voxel_width();
  int* max_number_neighbors = normalManager->get_knn_max_nn();
  int* voxel_searchSize = normalManager->get_knn_voxel_search();

  //Optim gn
  double*PTP_distance_max = gnManager->get_dist_residual_max();
  int* iter_max = gnManager->get_iter_max();

  //Local map
  double* map_voxel_size = mapManager->get_map_voxel_size();
  double* min_root_distance = mapManager->get_min_root_distance();
  double* max_root_distance = mapManager->get_max_root_distance();
  double* max_voxel_distance = mapManager->get_max_voxel_distance();
  double* min_voxel_distance = mapManager->get_min_voxel_distance();
  double* grid_voxel_size = mapManager->get_grid_voxel_size();
  int* map_voxel_capacity = mapManager->get_map_voxel_capacity();

  //Slam
  *solver_gn = true;
  *solver_ceres = false;
  *thres_ego_trans = 2.0f;
  *thres_ego_rotat = 15.0f;
  *thres_pose_trans = 4.0f;
  *thres_pose_rotat = 15.0f;
  *thres_optimMinNorm = 0.4f;
  *nb_residual_min = 100;
  *nb_rlt_previous_mean = 10;
  *nb_rlt_previous_pose = 4;

  //Normal
  *size_voxelMap = 1.0f;
  *max_number_neighbors = 20;
  *voxel_searchSize = 1;

  //Optim gn
  *PTP_distance_max = 0.5f;
  *iter_max = 5;

  //Local map
  *map_voxel_size = 1.0f;
  *min_root_distance = 5.0f;
  *max_root_distance = 100.0f;
  *max_voxel_distance = 150.0f;
  *min_voxel_distance = 0.05f;
  *grid_voxel_size = 1.0f;
  *map_voxel_capacity = 20;

  //Specific
  this->set_nb_thread(8);

  //---------------------------
}
void SLAM_parameter::make_config_1(){
  //velodyne_vlp16
  //---------------------------

  //Slam
  bool* solver_gn = optimManager->get_solver_gn();
  bool* solver_ceres = optimManager->get_solver_ceres();
  double* thres_ego_trans = assessManager->get_thres_ego_trans();
  double* thres_ego_rotat = assessManager->get_thres_ego_rotat();
  double* thres_pose_trans = assessManager->get_thres_pose_trans();
  double* thres_pose_rotat = assessManager->get_thres_pose_rotat();
  double* thres_optimMinNorm = assessManager->get_thres_optimMinNorm();
  int* nb_residual_min = assessManager->get_nb_residual_min();
  int* nb_rlt_previous_mean = assessManager->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = assessManager->get_nb_rlt_previous_pose();

  //Normal
  double* size_voxelMap = normalManager->get_knn_voxel_width();
  int* max_number_neighbors = normalManager->get_knn_max_nn();
  int* voxel_searchSize = normalManager->get_knn_voxel_search();

  //Optim gn
  double*PTP_distance_max = gnManager->get_dist_residual_max();
  int* iter_max = gnManager->get_iter_max();

  //Local map
  double* map_voxel_size = mapManager->get_map_voxel_size();
  double* min_root_distance = mapManager->get_min_root_distance();
  double* max_root_distance = mapManager->get_max_root_distance();
  double* max_voxel_distance = mapManager->get_max_voxel_distance();
  double* min_voxel_distance = mapManager->get_min_voxel_distance();
  double* grid_voxel_size = mapManager->get_grid_voxel_size();
  int* map_voxel_capacity = mapManager->get_map_voxel_capacity();

  //Slam
  *solver_gn = true;
  *solver_ceres = false;

  //Assessment
  *thres_ego_trans = 2.0f;
  *thres_ego_rotat = 15.0f;
  *thres_pose_trans = 5.0f;
  *thres_pose_rotat = 15.0f;
  *thres_optimMinNorm = 0.3f;
  *nb_residual_min = 50;
  *nb_rlt_previous_mean = 10;
  *nb_rlt_previous_pose = 4;

  //Normal
  *size_voxelMap = 0.5f;
  *max_number_neighbors = 50;
  *voxel_searchSize = 1;

  //Optim gn
  *PTP_distance_max = 0.7f;
  *iter_max = 5;

  //Local map
  *map_voxel_size = 0.5f;
  *min_root_distance = 0.0f;
  *max_root_distance = 100.0f;
  *max_voxel_distance = 150.0f;
  *min_voxel_distance = 0.05f;
  *grid_voxel_size = 0.06f;
  *map_voxel_capacity = 20;

  //Specific
  this->set_nb_thread(8);

  //---------------------------
}
void SLAM_parameter::make_config_2(){
  //velodyne_hdl32
  //---------------------------

  //Slam
  bool* solver_gn = optimManager->get_solver_gn();
  bool* solver_ceres = optimManager->get_solver_ceres();
  double* thres_ego_trans = assessManager->get_thres_ego_trans();
  double* thres_ego_rotat = assessManager->get_thres_ego_rotat();
  double* thres_pose_trans = assessManager->get_thres_pose_trans();
  double* thres_pose_rotat = assessManager->get_thres_pose_rotat();
  double* thres_optimMinNorm = assessManager->get_thres_optimMinNorm();
  int* nb_residual_min = assessManager->get_nb_residual_min();
  int* nb_rlt_previous_mean = assessManager->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = assessManager->get_nb_rlt_previous_pose();

  //Normal
  double* size_voxelMap = normalManager->get_knn_voxel_width();
  int* max_number_neighbors = normalManager->get_knn_max_nn();
  int* voxel_searchSize = normalManager->get_knn_voxel_search();

  //Optim gn
  double*PTP_distance_max = gnManager->get_dist_residual_max();
  int* iter_max = gnManager->get_iter_max();

  //Local map
  double* map_voxel_size = mapManager->get_map_voxel_size();
  double* min_root_distance = mapManager->get_min_root_distance();
  double* max_root_distance = mapManager->get_max_root_distance();
  double* max_voxel_distance = mapManager->get_max_voxel_distance();
  double* min_voxel_distance = mapManager->get_min_voxel_distance();
  double* grid_voxel_size = mapManager->get_grid_voxel_size();
  int* map_voxel_capacity = mapManager->get_map_voxel_capacity();

  //Slam
  *solver_gn = true;
  *solver_ceres = false;

  //Assessment
  *thres_ego_trans = 2.0f;
  *thres_ego_rotat = 15.0f;
  *thres_pose_trans = 4.0f;
  *thres_pose_rotat = 15.0f;
  *thres_optimMinNorm = 0.4f;
  *nb_residual_min = 100;
  *nb_rlt_previous_mean = 10;
  *nb_rlt_previous_pose = 4;

  //Normal
  *size_voxelMap = 1.0f;
  *max_number_neighbors = 20;
  *voxel_searchSize = 1;

  //Optim gn
  *PTP_distance_max = 0.5f;
  *iter_max = 5;

  //Local map
  *map_voxel_size = 1.0f;
  *min_root_distance = 5.0f;
  *max_root_distance = 100.0f;
  *max_voxel_distance = 150.0f;
  *min_voxel_distance = 0.05f;
  *grid_voxel_size = 1.0f;
  *map_voxel_capacity = 20;

  //Specific
  this->set_nb_thread(8);

  //---------------------------
}

//Setting functions
void SLAM_parameter::set_predefined_conf(int conf){
  //---------------------------

  switch(conf){
    case 0:{//VLP-64
      this->make_config_default();
      break;
    }
    case 1:{//VLP-64
      this->make_config_0();
      break;
    }
    case 2:{//VLP-16
      this->make_config_1();
      break;
    }
    case 3:{//HDL-32
      this->make_config_2();
      break;
    }
  }

  //---------------------------
  this->predefined_conf = conf;
}
void SLAM_parameter::set_nb_thread(int value){
  //---------------------------

  slamManager->set_nb_thread(value);
  normalManager->set_nb_thread(value);
  gnManager->set_nb_thread(value);

  //---------------------------
}
