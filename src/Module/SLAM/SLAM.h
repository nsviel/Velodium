#ifndef SLAM_H
#define SLAM_H

#include "../../Engine/Data/struct_voxelMap.h"
#include "../../common.h"

class Engine_node;
class Scene;
class Object;
class Configuration;

class SLAM_init;
class SLAM_optim;
class SLAM_assessment;
class SLAM_map;
class SLAM_parameter;
class SLAM_normal;


class SLAM
{
public:
  //Constructor / Destructor
  SLAM(Engine_node* node);
  ~SLAM();

public:
  void update_configuration();
  bool compute_slam(Cloud* cloud, int subset_ID);
  void reset_slam();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline SLAM_normal* get_slam_normal(){return slam_normal;}
  inline SLAM_optim* get_slam_optim(){return slam_optim;}
  inline SLAM_assessment* get_slam_assess(){return slam_assess;}
  inline SLAM_map* get_slam_map(){return slam_map;}
  inline SLAM_parameter* get_slam_param(){return slam_param;}

  inline void set_nb_thread(int value){this->nb_thread = value;}
  inline void set_offline_ID_max(int value){offline_ID_max = value;}

private:
  bool check_condition(Cloud* cloud, int subset_ID);
  void compute_finalization(Cloud* cloud, int subset_ID, bool success, float t_begin);
  void update_subset_location(Subset* subset);
  void update_subset_glyph(Subset* subset);
  void reset_visibility(Cloud* cloud, int subset_ID);

private:
  Engine_node* node_engine;
  Scene* sceneManager;
  Object* objectManager;
  Configuration* configManager;

  //SLAM_optim_ceres* ceresManager;
  SLAM_init* slam_init;
  SLAM_optim* slam_optim;
  SLAM_normal* slam_normal;
  SLAM_assessment* slam_assess;
  SLAM_map* slam_map;
  SLAM_parameter* slam_param;

  string lidar_model;
  int offline_ID_max;
  int nb_thread;
};

#endif
