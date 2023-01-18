#include "SLAM_parameter.h"

#include "SLAM_assessment.h"
#include "SLAM_map.h"
#include "SLAM_transform.h"
#include "SLAM_sampling.h"

#include "../optim/SLAM_optim.h"
#include "../optim/SLAM_optim_gn.h"
#include "../optim/SLAM_normal.h"

#include "SLAM.h"

//Config 0 -> vlp_16
//Config 1 -> vlp_64
//Config 2 -> hdl_32


//Constructor / Destructor
SLAM_parameter::SLAM_parameter(SLAM* slam){
  //---------------------------

  this->slamManager = slam;
  this->slam_normal = slam->get_slam_normal();
  this->slam_optim = slam->get_slam_optim();
  this->slam_optim_gn = slam_optim->get_optim_gn();
  this->slam_assess = slam->get_slam_assess();
  this->slam_map = slam->get_slam_map();
  this->slam_transf = slam->get_slam_transf();
  this->slam_sampling = slam->get_slam_sampling();

  this->predefined_conf = 0;

  //---------------------------
}
SLAM_parameter::~SLAM_parameter(){}

//Configuration function
void SLAM_parameter::make_config(int conf){
  //---------------------------

  switch(conf){
    case 0:{ //VLP16
      this->make_config_1();
      break;
    }
    case 1:{ //VLP64
      this->make_config_0();
      break;
    }
    case 2:{ //HDL32
      this->make_config_2();
      break;
    }
    case 3:{ //Train
      this->make_config_3();
      break;
    }
  }

  //---------------------------
  this->predefined_conf = conf;
}
void SLAM_parameter::make_config(string conf){
  //---------------------------

  if(conf == "velodyne_vlp16" || conf == "scala"){
    this->make_config_1();
    this->predefined_conf = 0;
  }
  else if(conf == "velodyne_vlp64"){
    this->make_config_0();
    this->predefined_conf = 1;
  }
  else if(conf == "velodyne_hdl32"){
    this->make_config_2();
    this->predefined_conf = 2;
  }
  else if(conf == "velodyne_vlp16_reduced"){
    this->make_config_3();
    this->predefined_conf = 3;
  }

  //---------------------------
}

//Specific function
void SLAM_parameter::make_config_default(){
  //velodyne_vlp64
  //---------------------------

  //Assessment
  double* thres_ego_trans = slam_assess->get_thres_ego_trans();
  double* thres_ego_rotat = slam_assess->get_thres_ego_rotat();
  double* thres_pose_trans = slam_assess->get_thres_pose_trans();
  double* thres_pose_rotat = slam_assess->get_thres_pose_rotat();
  double* thres_optimMinNorm = slam_assess->get_thres_optimMinNorm();
  int* nb_residual_min = slam_assess->get_nb_residual_min();
  int* nb_rlt_previous_mean = slam_assess->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = slam_assess->get_nb_rlt_previous_pose();

  //Normal
  double* size_voxelMap = slam_normal->get_knn_voxel_width();
  int* max_number_neighbors = slam_normal->get_knn_max_nn();
  int* voxel_searchSize = slam_normal->get_knn_voxel_search();

  //Optimization
  bool* solver_gn = slam_optim->get_solver_gn();
  bool* solver_ceres = slam_optim->get_solver_ceres();
  double*PTP_distance_max = slam_optim_gn->get_dist_residual_max();
  int* iter_max = slam_optim_gn->get_iter_max();

  //Local map
  slamap* local_map = slam_map->get_local_map();

  //Transformation
  double* min_root_distance = slam_sampling->get_min_root_distance();
  double* max_root_distance = slam_sampling->get_max_root_distance();
  double* grid_voxel_size = slam_sampling->get_grid_voxel_size();

  //Transformation
  *grid_voxel_size = 1.0f;
  *min_root_distance = 5.0f;
  *max_root_distance = 100.0f;

  //Assesment
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

  //Optimization
  *solver_gn = true;
  *solver_ceres = false;
  *PTP_distance_max = 0.3f;
  *iter_max = 5;

  //Local map
  local_map->voxel_width = 1.0f;
  local_map->voxel_max_dist = 150.0f;
  local_map->voxel_min_point_dist = 0.1;
  local_map->voxel_capacity = 20;

  //Specific
  this->set_nb_thread(1);

  //---------------------------
}
void SLAM_parameter::make_config_0(){
  //velodyne_vlp64
  //---------------------------

  //Assessment
  double* thres_ego_trans = slam_assess->get_thres_ego_trans();
  double* thres_ego_rotat = slam_assess->get_thres_ego_rotat();
  double* thres_pose_trans = slam_assess->get_thres_pose_trans();
  double* thres_pose_rotat = slam_assess->get_thres_pose_rotat();
  double* thres_optimMinNorm = slam_assess->get_thres_optimMinNorm();
  int* nb_residual_min = slam_assess->get_nb_residual_min();
  int* nb_rlt_previous_mean = slam_assess->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = slam_assess->get_nb_rlt_previous_pose();

  //Normal
  double* size_voxelMap = slam_normal->get_knn_voxel_width();
  int* max_number_neighbors = slam_normal->get_knn_max_nn();
  int* voxel_searchSize = slam_normal->get_knn_voxel_search();

  //Optimization
  bool* solver_gn = slam_optim->get_solver_gn();
  bool* solver_ceres = slam_optim->get_solver_ceres();
  double*PTP_distance_max = slam_optim_gn->get_dist_residual_max();
  int* iter_max = slam_optim_gn->get_iter_max();

  //Local map
  slamap* local_map = slam_map->get_local_map();

  //Transformation
  double* min_root_distance = slam_sampling->get_min_root_distance();
  double* max_root_distance = slam_sampling->get_max_root_distance();
  double* grid_voxel_size = slam_sampling->get_grid_voxel_size();
  int* max_keypoint = slam_sampling->get_max_keypoint();

  //Transformation
  *min_root_distance = 5.0f;
  *max_root_distance = 100.0f;
  *grid_voxel_size = 1.0f;
  *max_keypoint = 2000;

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

  //Optimization
  *solver_gn = true;
  *solver_ceres = false;
  *PTP_distance_max = 0.5f;
  *iter_max = 5;

  //Local map
  local_map->voxel_width = 1.0f;
  local_map->voxel_max_dist = 150.0f;
  local_map->voxel_min_point_dist = 0.05f;
  local_map->voxel_capacity = 20;

  //Specific
  this->set_nb_thread(8);

  //---------------------------
}
void SLAM_parameter::make_config_1(){
  //velodyne_vlp16
  //---------------------------

  //Assessment
  double* thres_ego_trans = slam_assess->get_thres_ego_trans();
  double* thres_ego_rotat = slam_assess->get_thres_ego_rotat();
  double* thres_pose_trans = slam_assess->get_thres_pose_trans();
  double* thres_pose_rotat = slam_assess->get_thres_pose_rotat();
  double* thres_optimMinNorm = slam_assess->get_thres_optimMinNorm();
  int* nb_residual_min = slam_assess->get_nb_residual_min();
  int* nb_rlt_previous_mean = slam_assess->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = slam_assess->get_nb_rlt_previous_pose();

  //Normal
  double* size_voxelMap = slam_normal->get_knn_voxel_width();
  int* max_number_neighbors = slam_normal->get_knn_max_nn();
  int* voxel_searchSize = slam_normal->get_knn_voxel_search();

  //Optimization
  bool* solver_gn = slam_optim->get_solver_gn();
  bool* solver_ceres = slam_optim->get_solver_ceres();
  double*PTP_distance_max = slam_optim_gn->get_dist_residual_max();
  int* iter_max = slam_optim_gn->get_iter_max();

  //Local map
  slamap* local_map = slam_map->get_local_map();

  //Transformation
  double* min_root_distance = slam_sampling->get_min_root_distance();
  double* max_root_distance = slam_sampling->get_max_root_distance();
  double* grid_voxel_size = slam_sampling->get_grid_voxel_size();

  //Transformation
  *min_root_distance = 1;
  *max_root_distance = 100.0f;
  *grid_voxel_size = 0.1f;

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
  *max_number_neighbors = 40;
  *voxel_searchSize = 1;

  //Optimization
  *solver_gn = true;
  *solver_ceres = false;
  *PTP_distance_max = 0.7f;
  *iter_max = 5;

  //Local map
  local_map->voxel_width = 0.5f;
  local_map->voxel_max_dist = 150.0f;
  local_map->voxel_min_point_dist = 0.05f;
  local_map->voxel_capacity = 20;

  //Specific
  this->set_nb_thread(8);

  //---------------------------
}
void SLAM_parameter::make_config_2(){
  //velodyne_hdl32
  //---------------------------

  //Assessment
  double* thres_ego_trans = slam_assess->get_thres_ego_trans();
  double* thres_ego_rotat = slam_assess->get_thres_ego_rotat();
  double* thres_pose_trans = slam_assess->get_thres_pose_trans();
  double* thres_pose_rotat = slam_assess->get_thres_pose_rotat();
  double* thres_optimMinNorm = slam_assess->get_thres_optimMinNorm();
  int* nb_residual_min = slam_assess->get_nb_residual_min();
  int* nb_rlt_previous_mean = slam_assess->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = slam_assess->get_nb_rlt_previous_pose();

  //Normal
  double* size_voxelMap = slam_normal->get_knn_voxel_width();
  int* max_number_neighbors = slam_normal->get_knn_max_nn();
  int* voxel_searchSize = slam_normal->get_knn_voxel_search();

  //Optimization
  bool* solver_gn = slam_optim->get_solver_gn();
  bool* solver_ceres = slam_optim->get_solver_ceres();
  double* PTP_distance_max = slam_optim_gn->get_dist_residual_max();
  int* iter_max = slam_optim_gn->get_iter_max();

  //Local map
  slamap* local_map = slam_map->get_local_map();

  //Transformation
  double* min_root_distance = slam_sampling->get_min_root_distance();
  double* max_root_distance = slam_sampling->get_max_root_distance();
  double* grid_voxel_size = slam_sampling->get_grid_voxel_size();

  //Transformation
  *min_root_distance = 5.0f;
  *max_root_distance = 100.0f;
  *grid_voxel_size = 1.0f;

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

  //Optimization
  *solver_gn = true;
  *solver_ceres = false;
  *PTP_distance_max = 0.5f;
  *iter_max = 5;

  //Local map
  local_map->voxel_width = 1;
  local_map->voxel_max_dist = 150.0f;
  local_map->voxel_min_point_dist = 0.05f;
  local_map->voxel_capacity = 20;

  //Specific
  this->set_nb_thread(8);

  //---------------------------
}
void SLAM_parameter::make_config_3(){
  //vlp16 with narrow pov
  //---------------------------

  //Assessment
  double* thres_ego_trans = slam_assess->get_thres_ego_trans();
  double* thres_ego_rotat = slam_assess->get_thres_ego_rotat();
  double* thres_pose_trans = slam_assess->get_thres_pose_trans();
  double* thres_pose_rotat = slam_assess->get_thres_pose_rotat();
  double* thres_optimMinNorm = slam_assess->get_thres_optimMinNorm();
  double* thres_diff_angle = slam_assess->get_thres_diff_angle();
  int* nb_residual_min = slam_assess->get_nb_residual_min();
  int* nb_rlt_previous_mean = slam_assess->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = slam_assess->get_nb_rlt_previous_pose();

  //Normal
  double* size_voxelMap = slam_normal->get_knn_voxel_width();
  int* max_number_neighbors = slam_normal->get_knn_max_nn();
  int* voxel_searchSize = slam_normal->get_knn_voxel_search();

  //Optimization
  bool* solver_gn = slam_optim->get_solver_gn();
  bool* solver_ceres = slam_optim->get_solver_ceres();
  double* PTP_distance_max = slam_optim_gn->get_dist_residual_max();
  int* iter_max = slam_optim_gn->get_iter_max();

  //Local map
  slamap* local_map = slam_map->get_local_map();

  //Transformation
  double* min_root_distance = slam_sampling->get_min_root_distance();
  double* max_root_distance = slam_sampling->get_max_root_distance();
  double* grid_voxel_size = slam_sampling->get_grid_voxel_size();
  int* max_keypoint = slam_sampling->get_max_keypoint();

  //Transformation
  *min_root_distance = 3.0f;
  *max_root_distance = 200.0f;
  *grid_voxel_size = 0.3;
  *max_keypoint = 2500;

  //Assessment
  *thres_ego_trans = 10.0f;
  *thres_ego_rotat = 15.0f;
  *thres_pose_trans = 10.0f;
  *thres_pose_rotat = 15.0f;
  *thres_optimMinNorm = 0.4f;
  *nb_residual_min = 100;
  *nb_rlt_previous_mean = 10;
  *nb_rlt_previous_pose = 10;
  *thres_diff_angle = 10;

  //Normal
  *size_voxelMap = 0.5f;
  *max_number_neighbors = 40;
  *voxel_searchSize = 1;

  //Optimization
  *solver_gn = true;
  *solver_ceres = false;
  *PTP_distance_max = 10;
  *iter_max = 5;

  //Local map
  local_map->voxel_width = 1;
  local_map->voxel_max_dist = 150.0f;
  local_map->voxel_min_point_dist = 0.001;
  local_map->voxel_capacity = 100;

  //Specific
  this->set_nb_thread(8);

  //---------------------------
}

//Setting function
void SLAM_parameter::set_predefined_conf(int conf){
  //---------------------------

  switch(conf){
    case 0:{//VLP-16
      this->make_config_1();
      break;
    }
    case 1:{//VLP-64
      this->make_config_0();
      break;
    }
    case 2:{//HDL-32
      this->make_config_2();
      break;
    }
    case 3:{//Train
      this->make_config_3();
      break;
    }
  }

  //---------------------------
  this->predefined_conf = conf;
}
void SLAM_parameter::set_nb_thread(int value){
  //---------------------------

  slamManager->set_nb_thread(value);
  slam_normal->set_nb_thread(value);
  slam_optim_gn->set_nb_thread(value);

  //---------------------------
}
