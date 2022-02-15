#ifndef SLAM_CONFIGURATION_H
#define SLAM_CONFIGURATION_H

#include "../../../common.h"

class Slam;
class SLAM_normal;
class SLAM_optim_gn;
class SLAM_assessment;
class SLAM_localMap;


class SLAM_configuration
{
public:
  //Constructor / Destructor
  SLAM_configuration(Slam* slam);
  ~SLAM_configuration();

public:
  //Main function
  void make_config(int conf);

  //Subfunctions
  void make_config_0();
  void make_config_1();
  void set_predefined_conf(int conf);
  void set_nb_thread(int value);

  inline int* get_predefined_conf(){return &predefined_conf;}

private:
  Slam* slamManager;
  SLAM_normal* normalManager;
  SLAM_optim_gn* gnManager;
  SLAM_assessment* assessManager;
  SLAM_localMap* mapManager;

  int predefined_conf;
};


#endif
