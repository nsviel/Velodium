#ifndef SLAM_H
#define SLAM_H

#include "CT_ICP/SLAM_normal.h"
#include "Cost_function/Cost_function.h"

#include "../../Engine/Data/struct_voxelMap.h"
#include "../../common.h"

class Engine_node;
class Scene;
class Glyphs;

class SLAM_optim_ceres;
class SLAM_optim_gn;
class SLAM_assessment;
class SLAM_localMap;
class SLAM_configuration;


class Slam
{
public:
  //Constructor / Destructor
  Slam(Engine_node* node);
  ~Slam();

public:
  void compute_slam(Cloud* cloud);
  void compute_slam_online(Cloud* cloud, int ID);
  void reset_slam();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline SLAM_normal* get_slam_normal(){return normalManager;}
  inline SLAM_optim_ceres* get_slam_ceres(){return ceresManager;}
  inline SLAM_optim_gn* get_slam_gn(){return gnManager;}
  inline SLAM_assessment* get_slam_assess(){return assessManager;}
  inline SLAM_localMap* get_slam_map(){return mapManager;}
  inline SLAM_configuration* get_slam_config(){return configManager;}
  inline SLAM_assessment* get_assessManager(){return assessManager;}

  inline bool* get_verbose(){return &verbose;}
  inline bool* get_solver_gn(){return &solver_GN;}
  inline bool* get_solver_ceres(){return &solver_ceres;}
  inline void set_nb_thread(int value){this->nb_thread = value;}
  inline void set_ID_max(int value){ID_max = value;}
  inline void set_ID_all(bool value){ID_all = value;}

private:
  bool check_conditions(Cloud* cloud, int subset_ID);
  void init_frameID(Cloud* cloud, int ID);
  void init_frameTimestamp(Subset* subset);
  void init_frameChain(Frame* frame, Frame* frame_m1, Frame* frame_m2);
  void init_distortion(Frame* frame);

  void compute_optimization(Frame* frame, Frame* frame_m1);
  void compute_assessment(Cloud* cloud, int ID);
  void compute_updateLocation(Subset* subset);
  void compute_statistics(float duration, Frame* frame, Frame* frame_m1, Subset* subset);

private:
  Engine_node* node_engine;
  Scene* sceneManager;
  Glyphs* glyphManager;

  SLAM_optim_ceres* ceresManager;
  SLAM_optim_gn* gnManager;
  SLAM_normal* normalManager;
  SLAM_assessment* assessManager;
  SLAM_localMap* mapManager;
  SLAM_configuration* configManager;

  bool ID_all;
  bool solver_GN, solver_ceres;
  bool verbose;
  int map_size_old;
  int map_frame_ID;
  int map_frame_begin_ID;
  int ID_cloud;
  int ID_max;
  int nb_thread;
};

#endif
