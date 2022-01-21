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
class SLAM_assessment;
class SLAM_localMap;


class CT_ICP
{
public:
  //Constructor / Destructor
  CT_ICP();
  ~CT_ICP();

public:
  void compute_slam(Cloud* cloud);
  void compute_slam_online(Cloud* cloud, int i);

  void set_nb_thread(int value);
  void reset();

  inline SLAM_normal* get_SLAM_normal(){return normalManager;}
  inline SLAM_optim_ceres* get_SLAM_optim_ceres(){return ceresManager;}
  inline SLAM_optim_gn* get_SLAM_optim_gn(){return gnManager;}
  inline SLAM_assessment* get_assessManager(){return assessManager;}
  inline SLAM_localMap* get_mapManager(){return mapManager;}

  inline bool* get_verbose(){return &verbose;}
  inline void set_frame_max(int value){frame_max = value;}
  inline void set_frame_all(bool value){frame_all = value;}

private:
  void init_frameID(Frame* frame, int i);
  void init_frameTimestamp(Subset* subset);
  void init_frameChain(Frame* frame, Frame* frame_m1, Frame* frame_m2);
  void init_distortion(Frame* frame);

  void compute_optimization(Frame* frame, Frame* frame_m1);
  void compute_assessment(Cloud* cloud, int i);
  void compute_updateLocation(Subset* subset);
  void compute_statistics(float duration, Frame* frame, Frame* frame_m1, Subset* subset);

private:
  Scene* sceneManager;
  Glyphs* glyphManager;

  SLAM_optim_ceres* ceresManager;
  SLAM_optim_gn* gnManager;
  SLAM_normal* normalManager;
  SLAM_assessment* assessManager;
  SLAM_localMap* mapManager;

  bool frame_all;
  bool solver_GN, solver_ceres;
  bool verbose;
  int map_size_old;
  int map_frame_ID;
  int map_frame_begin_ID;
  int frame_max;
  int nb_thread;
};

#endif
