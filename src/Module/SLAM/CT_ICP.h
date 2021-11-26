#ifndef CT_ICP_H
#define CT_ICP_H

#include "CT_ICP/SLAM_normal.h"

#include "Cost_function/Cost_function.h"

#include "../../common.h"

#include <map>

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

  void init_frameTimestamp(Subset* subset);
  void init_frameChain(Frame* frame, Frame* frame_m1, Frame* frame_m2);

  void compute_gridSampling(Subset* subset);
  void compute_normal(Subset* subset);
  void compute_optimization(Frame* frame, Frame* frame_m1);

  void add_keypointsToCloud(Subset* subset);
  void add_pointsToLocalMap(Frame* frame);

  inline float* get_sampling_size(){return &sampling_size;}
  inline SLAM_optim_ceres* get_SLAM_optim_ceres(){return ceresManager;}

private:
  SLAM_optim_ceres* ceresManager;
  SLAM_optim_gn* gnManager;
  SLAM_normal* normalManager;

  voxelMap map;
  float sampling_size;
  bool solver_GN, solver_ceres;
  float size_voxelMap;
  int voxel_sizeMax;
};

#endif
