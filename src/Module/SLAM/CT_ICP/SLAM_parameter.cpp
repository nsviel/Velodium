#include "SLAM_parameter.h"

#include "../Slam.h"
#include "SLAM_normal.h"
#include "SLAM_optim_gn.h"
#include "SLAM_assessment.h"
#include "SLAM_localMap.h"


//Constructor / Destructor
SLAM_parameter::SLAM_parameter(Slam* slam){
  //---------------------------

  this->slamManager = slam;
  this->normalManager = slam->get_slam_normal();
  this->gnManager = slam->get_slam_gn();
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
    case 0:{ //VLP64
      this->make_config_0();
      break;
    }
    case 1:{ //VLP16
      this->make_config_1();
      break;
    }
    case 2:{ //HDL32
      this->make_config_2();
      break;
    }
  }

  //---------------------------
  this->predefined_conf = conf;
}
void SLAM_parameter::make_config(string conf){
  //---------------------------

  if(conf == "velodyne_vlp64"){
    this->make_config_0();
    this->predefined_conf = 0;
  }
  else if(conf == "velodyne_vlp16" || conf == "scala"){
    this->make_config_1();
    this->predefined_conf = 1;
  }
  else if(conf == "velodyne_hdl32"){
    this->make_config_2();
    this->predefined_conf = 2;
  }

  //---------------------------
}
void SLAM_parameter::make_config_0(){
  //---------------------------

  //Slam
  bool* solver_gn = slamManager->get_solver_gn();
  bool* solver_ceres = slamManager->get_solver_ceres();
  *solver_gn = true;
  *solver_ceres = false;

  //Assessment
  float* thres_ego_trans = assessManager->get_thres_ego_trans();
  float* thres_ego_rotat = assessManager->get_thres_ego_rotat();
  float* thres_pose_trans = assessManager->get_thres_pose_trans();
  float* thres_pose_rotat = assessManager->get_thres_pose_rotat();
  float* thres_optimMinNorm = assessManager->get_thres_optimMinNorm();
  int* nb_residual_min = assessManager->get_nb_residual_min();
  int* nb_rlt_previous_mean = assessManager->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = assessManager->get_nb_rlt_previous_pose();

  *thres_ego_trans = 2.0f;
  *thres_ego_rotat = 15.0f;
  *thres_pose_trans = 3.0f;
  *thres_pose_rotat = 15.0f;
  *thres_optimMinNorm = 0.2f;
  *nb_residual_min = 100;
  *nb_rlt_previous_mean = 10;
  *nb_rlt_previous_pose = 4;

  //Normal
  float* size_voxelMap = normalManager->get_size_voxelMap();
  int* max_number_neighbors = normalManager->get_max_number_neighbors();
  int* voxel_searchSize = normalManager->get_voxel_searchSize();

  *size_voxelMap = 1.0f;
  *max_number_neighbors = 20;
  *voxel_searchSize = 1;

  //Optim gn
  float*PTP_distance_max = gnManager->get_PTP_distance_max();
  int* iter_max = gnManager->get_iter_max();

  *PTP_distance_max = 0.5f;
  *iter_max = 5;

  //Local map
  float* voxel_size_localMap = mapManager->get_voxel_size_localMap();
  float* voxel_size_slamMap = mapManager->get_voxel_size_slamMap();
  float* min_root_distance = mapManager->get_min_root_distance();
  float* max_root_distance = mapManager->get_max_root_distance();
  float* max_voxel_distance = mapManager->get_max_voxel_distance();
  float* min_voxel_distance = mapManager->get_min_voxel_distance();
  float* grid_voxel_size = mapManager->get_grid_voxel_size();
  int* map_max_voxelNbPoints = mapManager->get_map_max_voxelNbPoints();

  *voxel_size_localMap = 1.0f;
  *voxel_size_slamMap = 0.5;
  *min_root_distance = 5.0f;
  *max_root_distance = 100.0f;
  *max_voxel_distance = 150.0f;
  *min_voxel_distance = 0.05f;
  *grid_voxel_size = 1.0f;
  *map_max_voxelNbPoints = 20;

  //Specific functions
  this->set_nb_thread(8);

  //---------------------------
}
void SLAM_parameter::make_config_1(){
  //---------------------------

  //Slam
  bool* solver_gn = slamManager->get_solver_gn();
  bool* solver_ceres = slamManager->get_solver_ceres();
  *solver_gn = true;
  *solver_ceres = false;

  //Assessment
  float* thres_ego_trans = assessManager->get_thres_ego_trans();
  float* thres_ego_rotat = assessManager->get_thres_ego_rotat();
  float* thres_pose_trans = assessManager->get_thres_pose_trans();
  float* thres_pose_rotat = assessManager->get_thres_pose_rotat();
  float* thres_optimMinNorm = assessManager->get_thres_optimMinNorm();
  int* nb_residual_min = assessManager->get_nb_residual_min();
  int* nb_rlt_previous_mean = assessManager->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = assessManager->get_nb_rlt_previous_pose();

  *thres_ego_trans = 2.0f;
  *thres_ego_rotat = 15.0f;
  *thres_pose_trans = 3.0f;
  *thres_pose_rotat = 15.0f;
  *thres_optimMinNorm = 0.2f;
  *nb_residual_min = 50;
  *nb_rlt_previous_mean = 10;
  *nb_rlt_previous_pose = 4;

  //Normal
  float* size_voxelMap = normalManager->get_size_voxelMap();
  int* max_number_neighbors = normalManager->get_max_number_neighbors();
  int* voxel_searchSize = normalManager->get_voxel_searchSize();

  *size_voxelMap = 0.5f;
  *max_number_neighbors = 50;
  *voxel_searchSize = 1;

  //Optim gn
  float*PTP_distance_max = gnManager->get_PTP_distance_max();
  int* iter_max = gnManager->get_iter_max();

  *PTP_distance_max = 0.7f;
  *iter_max = 5;

  //Local map
  float* voxel_size_localMap = mapManager->get_voxel_size_localMap();
  float* voxel_size_slamMap = mapManager->get_voxel_size_slamMap();
  float* min_root_distance = mapManager->get_min_root_distance();
  float* max_root_distance = mapManager->get_max_root_distance();
  float* max_voxel_distance = mapManager->get_max_voxel_distance();
  float* min_voxel_distance = mapManager->get_min_voxel_distance();
  float* grid_voxel_size = mapManager->get_grid_voxel_size();
  int* map_max_voxelNbPoints = mapManager->get_map_max_voxelNbPoints();

  *voxel_size_localMap = 0.5f;
  *voxel_size_slamMap = 0.5f;
  *min_root_distance = 0.0f;
  *max_root_distance = 100.0f;
  *max_voxel_distance = 150.0f;
  *min_voxel_distance = 0.05f;
  *grid_voxel_size = 0.06f;
  *map_max_voxelNbPoints = 20;

  //Specific functions
  this->set_nb_thread(8);

  //---------------------------
}
void SLAM_parameter::make_config_2(){
  //---------------------------

  //Slam
  bool* solver_gn = slamManager->get_solver_gn();
  bool* solver_ceres = slamManager->get_solver_ceres();
  *solver_gn = true;
  *solver_ceres = false;

  //Assessment
  float* thres_ego_trans = assessManager->get_thres_ego_trans();
  float* thres_ego_rotat = assessManager->get_thres_ego_rotat();
  float* thres_pose_trans = assessManager->get_thres_pose_trans();
  float* thres_pose_rotat = assessManager->get_thres_pose_rotat();
  float* thres_optimMinNorm = assessManager->get_thres_optimMinNorm();
  int* nb_residual_min = assessManager->get_nb_residual_min();
  int* nb_rlt_previous_mean = assessManager->get_nb_rlt_previous_mean();
  int* nb_rlt_previous_pose = assessManager->get_nb_rlt_previous_pose();

  *thres_ego_trans = 2.0f;
  *thres_ego_rotat = 15.0f;
  *thres_pose_trans = 3.0f;
  *thres_pose_rotat = 15.0f;
  *thres_optimMinNorm = 0.2f;
  *nb_residual_min = 50;
  *nb_rlt_previous_mean = 10;
  *nb_rlt_previous_pose = 4;

  //Normal
  float* size_voxelMap = normalManager->get_size_voxelMap();
  int* max_number_neighbors = normalManager->get_max_number_neighbors();
  int* voxel_searchSize = normalManager->get_voxel_searchSize();

  *size_voxelMap = 0.5f;
  *max_number_neighbors = 50;
  *voxel_searchSize = 1;

  //Optim gn
  float*PTP_distance_max = gnManager->get_PTP_distance_max();
  int* iter_max = gnManager->get_iter_max();

  *PTP_distance_max = 0.7f;
  *iter_max = 5;

  //Local map
  float* voxel_size_localMap = mapManager->get_voxel_size_localMap();
  float* voxel_size_slamMap = mapManager->get_voxel_size_slamMap();
  float* min_root_distance = mapManager->get_min_root_distance();
  float* max_root_distance = mapManager->get_max_root_distance();
  float* max_voxel_distance = mapManager->get_max_voxel_distance();
  float* min_voxel_distance = mapManager->get_min_voxel_distance();
  float* grid_voxel_size = mapManager->get_grid_voxel_size();
  int* map_max_voxelNbPoints = mapManager->get_map_max_voxelNbPoints();

  *voxel_size_localMap = 0.5f;
  *voxel_size_slamMap = 0.5f;
  *min_root_distance = 0.0f;
  *max_root_distance = 100.0f;
  *max_voxel_distance = 150.0f;
  *min_voxel_distance = 0.05f;
  *grid_voxel_size = 0.06f;
  *map_max_voxelNbPoints = 20;

  //Specific functions
  this->set_nb_thread(8);

  //---------------------------
}

//Setting functions
void SLAM_parameter::set_predefined_conf(int conf){
  //---------------------------

  switch(conf){
    case 0:{//64 fibers
      this->make_config_0();
      break;
    }
    case 1:{//16 fibers
      this->make_config_1();
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
