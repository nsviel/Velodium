#ifndef CT_ICP_H
#define CT_ICP_H

#include "CT_ICP/SLAM_normal.h"

#include "Cost_function/Cost_function.h"

#include "../../common.h"

#include <map>
#include <tsl/robin_map.h>
#include <tsl/robin_set.h>

class Scene;
class SLAM_optim_ceres;
class SLAM_optim_gn;


class CT_ICP
{
public:
  //Constructor / Destructor
  CT_ICP();
  ~CT_ICP();

public:
  void compute_slam();

  inline SLAM_normal* get_SLAM_normal(){return normalManager;}
  inline SLAM_optim_ceres* get_SLAM_optim_ceres(){return ceresManager;}
  inline SLAM_optim_gn* get_SLAM_optim_gn(){return gnManager;}
  inline float* get_sampling_width(){return &sampling_width;}
  inline float* get_mapVoxel_width(){return &map_voxel_width;}
  inline bool* get_verbose(){return &verbose;}
  inline bool* get_slamMap_voxelized(){return &slamMap_voxelized;}

  inline void set_frame_max(int value){frame_max = value;}
  inline void set_frame_all(bool value){frame_all = value;}

private:
  void init_frameTimestamp(Subset* subset);
  void init_frameChain(Frame* frame, Frame* frame_m1, Frame* frame_m2);
  void init_distortion(Frame* frame);

  void compute_gridSampling(Subset* subset);
  void compute_normal(Subset* subset);
  void compute_optimization(Frame* frame, Frame* frame_m1);

  void add_pointsToSlamMap(Subset* subset);
  void add_pointsToLocalMap(Frame* frame);

private:
  SLAM_optim_ceres* ceresManager;
  SLAM_optim_gn* gnManager;
  SLAM_normal* normalManager;
  Scene* sceneManager;
  voxelMap* map;

  float sampling_width;
  float map_voxel_width;
  int map_max_voxelNbPoints, frame_max;
  bool frame_all;
  bool solver_GN, solver_ceres;
  bool verbose, slamMap_voxelized;
};

#endif
