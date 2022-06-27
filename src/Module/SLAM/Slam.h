#ifndef SLAM_H
#define SLAM_H

#include "CT_ICP/SLAM_normal.h"
#include "Cost_function/Cost_function.h"

#include "../../Engine/Data/struct_voxelMap.h"
#include "../../common.h"

class Engine_node;
class Scene;
class Object;
class Configuration;

class SLAM_init;
class SLAM_optim_gn;
class SLAM_assessment;
class SLAM_localMap;
class SLAM_parameter;


class Slam
{
public:
  //Constructor / Destructor
  Slam(Engine_node* node);
  ~Slam();

public:
  void update_configuration();
  void compute_slam_offline(Cloud* cloud);
  void compute_slam_online(Cloud* cloud, int ID);
  void reset_slam();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline SLAM_normal* get_slam_normal(){return normalManager;}
  inline SLAM_optim_gn* get_slam_gn(){return gnManager;}
  inline SLAM_assessment* get_slam_assess(){return assessManager;}
  inline SLAM_localMap* get_slam_map(){return mapManager;}
  inline SLAM_parameter* get_slam_param(){return paramManager;}
  inline SLAM_assessment* get_assessManager(){return assessManager;}

  inline bool* get_verbose(){return &verbose;}
  inline bool* get_solver_gn(){return &solver_GN;}
  inline bool* get_solver_ceres(){return &solver_ceres;}
  inline void set_nb_thread(int value){this->nb_thread = value;}
  inline void set_offline_ID_max(int value){offline_ID_max = value;}

private:
  void compute_distortion(Frame* frame);
  void compute_optimization(Frame* frame, Frame* frame_m1);
  bool compute_assessment(Cloud* cloud, int ID);
  void compute_updateLocation_keypoint(Subset* subset);
  void compute_statistics(float duration, Frame* frame, Frame* frame_m1, Subset* subset);

  void update_subset_location(Subset* subset);
  void update_subset_glyph(Subset* subset);

  bool check_conditions(Cloud* cloud, int subset_ID);
  void reset_visibility(Cloud* cloud, int subset_ID);

private:
  Engine_node* node_engine;
  Scene* sceneManager;
  Object* objectManager;
  Configuration* configManager;

  //SLAM_optim_ceres* ceresManager;
  SLAM_init* initManager;
  SLAM_optim_gn* gnManager;
  SLAM_normal* normalManager;
  SLAM_assessment* assessManager;
  SLAM_localMap* mapManager;
  SLAM_parameter* paramManager;

  bool solver_GN;
  bool solver_ceres;
  bool verbose;
  bool with_distorsion;
  int map_size_old;
  int map_frame_begin_ID;
  int offline_ID_max;
  int nb_thread;
};

#endif
