#ifndef SLAM_OPTIM_H
#define SLAM_OPTIM_H

#include "../../../common.h"

class Scene;
class SLAM;
class SLAM_optim_gn;


class SLAM_optim
{
public:
  //Constructor / Destructor
  SLAM_optim(SLAM* slam);
  ~SLAM_optim();

public:
  //Main function
  void update_configuration();
  void compute_optimization(Cloud* cloud, int subset_ID);

  inline SLAM_optim_gn* get_optim_gn(){return slam_optim_gn;}
  inline bool* get_solver_gn(){return &solver_GN;}
  inline bool* get_solver_ceres(){return &solver_ceres;}

private:
  Scene* sceneManager;
  SLAM_optim_gn* slam_optim_gn;

  bool solver_GN;
  bool solver_ceres;
};


#endif
