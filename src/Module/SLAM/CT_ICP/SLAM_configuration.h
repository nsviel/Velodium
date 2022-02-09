#ifndef SLAM_CONFIGURATION_H
#define SLAM_CONFIGURATION_H

#include "../../../common.h"

class Slam;
class SLAM_normal;
class SLAM_optim_gn;


class SLAM_configuration
{
public:
  //Constructor / Destructor
  SLAM_configuration(Slam* slam);
  ~SLAM_configuration();

public:
  void set_nb_thread(int value);

private:
  Slam* slamManager;
  SLAM_normal* normalManager;
  SLAM_optim_gn* gnManager;
};


#endif
