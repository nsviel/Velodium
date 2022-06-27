#ifndef SLAM_PARAMETER_H
#define SLAM_PARAMETER_H

#include "../../../common.h"

class Slam;
class SLAM_normal;
class SLAM_optim;
class SLAM_optim_gn;
class SLAM_assessment;
class SLAM_map;


class SLAM_parameter
{
public:
  //Constructor / Destructor
  SLAM_parameter(Slam* slam);
  ~SLAM_parameter();

public:
  //Main function
  void make_config(int conf);
  void make_config(string conf);

  //Subfunctions
  void make_config_default();
  void make_config_0();
  void make_config_1();
  void make_config_2();

  //Setters
  void set_predefined_conf(int conf);
  void set_nb_thread(int value);

  inline int* get_predefined_conf(){return &predefined_conf;}

private:
  Slam* slamManager;
  SLAM_normal* normalManager;
  SLAM_optim* optimManager;
  SLAM_optim_gn* gnManager;
  SLAM_assessment* assessManager;
  SLAM_map* mapManager;

  int predefined_conf;
};


#endif
