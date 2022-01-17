#ifndef CT_ICP_H
#define CT_ICP_H

#include "struct_voxelMap.h"
#include "CT_ICP/SLAM_normal.h"
#include "Cost_function/Cost_function.h"

#include "../../common.h"

class Scene;
class Glyphs;
class SLAM_optim_ceres;
class SLAM_optim_gn;


class CT_ICP
{
public:
  //Constructor / Destructor
  CT_ICP();
  ~CT_ICP();

public:
  void compute_slam(Cloud* cloud);
  void compute_slam_online(Cloud* cloud, int i);

  float AngularDistance(Eigen::Matrix3d& rota, Eigen::Matrix3d& rotb);
  void set_nb_thread(int value);
  void reset();

  inline SLAM_normal* get_SLAM_normal(){return normalManager;}
  inline SLAM_optim_ceres* get_SLAM_optim_ceres(){return ceresManager;}
  inline SLAM_optim_gn* get_SLAM_optim_gn(){return gnManager;}
  inline float* get_voxel_size_gridMap(){return &voxel_size_gridMap;}
  inline float* get_voxel_size_localMap(){return &voxel_size_localMap;}
  inline float* get_voxel_size_slamMap(){return &voxel_size_slamMap;}
  inline float* get_min_subset_distance(){return &min_subset_distance;}
  inline float* get_max_subset_distance(){return &max_subset_distance;}
  inline float* get_max_voxel_distance(){return &max_voxel_distance;}
  inline float* get_min_voxel_distance(){return &min_voxel_distance;}
  inline float* get_thres_ego_trans(){return &thres_ego_trans;}
  inline float* get_thres_ego_rotat(){return &thres_ego_rotat;}
  inline float* get_thres_pose_trans(){return &thres_pose_trans;}
  inline float* get_thres_pose_rotat(){return &thres_pose_rotat;}
  inline float* get_thres_optimMinNorm(){return &thres_optimMinNorm;}
  inline bool* get_verbose(){return &verbose;}
  inline bool* get_slamMap_voxelized(){return &slamMap_voxelized;}
  inline int* get_map_max_voxelNbPoints(){return &map_max_voxelNbPoints;}

  inline void set_frame_max(int value){frame_max = value;}
  inline void set_frame_all(bool value){frame_all = value;}

private:
  void init_frameTimestamp(Subset* subset);
  void init_frameChain(Frame* frame, Frame* frame_m1, Frame* frame_m2);
  void init_distortion(Frame* frame);

  void compute_gridSampling(Subset* subset);
  void compute_optimization(Frame* frame, Frame* frame_m1);
  void compute_assessRegistration(Frame* frame, Frame* frame_m1);

  void add_pointsToSlamMap(Subset* subset);
  void add_pointsToLocalMap(Frame* frame);

  void end_updateSubsetLocation(Subset* subset);
  void end_clearTooFarVoxels(Eigen::Vector3d &current_location);
  void end_slamVoxelization(Cloud* cloud);
  void end_statistics(float duration, Frame* frame, Frame* frame_m1, Subset* subset);

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  
  SLAM_optim_ceres* ceresManager;
  SLAM_optim_gn* gnManager;
  SLAM_normal* normalManager;

  voxelMap* map;
  slamMap* map_cloud;

  float voxel_size_gridMap;
  float voxel_size_localMap;
  float voxel_size_slamMap;

  float min_subset_distance;
  float max_subset_distance;
  float max_voxel_distance;
  float min_voxel_distance;

  float thres_ego_trans;
  float thres_ego_rotat;
  float thres_pose_trans;
  float thres_pose_rotat;
  float thres_optimMinNorm;

  int map_max_voxelNbPoints;
  int map_size_old;
  int frame_max;
  int nb_thread;
  int frame_ID;
  bool frame_all;
  bool solver_GN, solver_ceres;
  bool verbose, slamMap_voxelized;
};

#endif
