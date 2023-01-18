#ifndef SLAM_PARAMETER_H
#define SLAM_PARAMETER_H

#include "../../../common.h"

class SLAM;
class SLAM_normal;
class SLAM_optim;
class SLAM_optim_gn;
class SLAM_assessment;
class SLAM_map;
class SLAM_transform;
class SLAM_sampling;


class SLAM_parameter
{
public:
  //Constructor / Destructor
  SLAM_parameter(SLAM* slam);
  ~SLAM_parameter();

public:
  //Configuration function
  void make_config(int conf);
  void make_config(string conf);

  //Specific function
  void make_config_default();
  void make_config_0();
  void make_config_1();
  void make_config_2();
  void make_config_3();

  //Setting function
  void set_predefined_conf(int conf);
  void set_nb_thread(int value);

  inline int* get_predefined_conf(){return &predefined_conf;}

private:
  SLAM* slamManager;
  SLAM_normal* slam_normal;
  SLAM_optim* slam_optim;
  SLAM_optim_gn* slam_optim_gn;
  SLAM_assessment* slam_assess;
  SLAM_map* slam_map;
  SLAM_transform* slam_transf;
  SLAM_sampling* slam_sampling;

  int predefined_conf;
};


#endif
