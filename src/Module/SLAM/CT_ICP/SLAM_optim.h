#ifndef SLAM_OPTIM_H
#define SLAM_OPTIM_H

#include "../../../common.h"

class Slam;
class SLAM_optim_gn;


class SLAM_optim
{
public:
  //Constructor / Destructor
  SLAM_optim(Slam* slam);
  ~SLAM_optim();

public:
  void update_configuration();
  void compute_distortion(Frame* frame);
  void compute_optimization(Frame* frame, Frame* frame_m1);

  inline SLAM_optim_gn* get_optim_gn(){return gnManager;}
  inline bool* get_solver_gn(){return &solver_GN;}
  inline bool* get_solver_ceres(){return &solver_ceres;}

private:
  SLAM_optim_gn* gnManager;

  bool with_distorsion;
  bool solver_GN;
  bool solver_ceres;
};


#endif
