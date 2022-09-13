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
class SLAM_optim;
class SLAM_assessment;
class SLAM_map;
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
  void reset_slam_hard();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline SLAM_normal* get_slam_normal(){return slam_normal;}
  inline SLAM_optim* get_slam_optim(){return slam_optim;}
  inline SLAM_assessment* get_slam_assess(){return slam_assess;}
  inline SLAM_map* get_slam_map(){return slam_map;}
  inline SLAM_parameter* get_slam_param(){return slam_param;}

  inline void set_nb_thread(int value){this->nb_thread = value;}
  inline void set_offline_ID_max(int value){offline_ID_max = value;}

private:
  bool compute_assessment(Cloud* cloud, int ID);
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
  SLAM_init* slam_init;
  SLAM_optim* slam_optim;
  SLAM_normal* slam_normal;
  SLAM_assessment* slam_assess;
  SLAM_map* slam_map;
  SLAM_parameter* slam_param;

  int offline_ID_max;
  int nb_thread;
};

#endif
