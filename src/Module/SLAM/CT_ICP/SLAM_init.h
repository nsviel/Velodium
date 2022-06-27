#ifndef SLAM_INIT_H
#define SLAM_INIT_H

#include "../../../common.h"

class Slam;
class SLAM_map;
class Scene;


class SLAM_init
{
public:
  //Constructor / Destructor
  SLAM_init(Slam* slam);
  ~SLAM_init();

public:
  //Main functions
  void compute_initialization(Cloud* cloud, int subset_ID);

  //Subfunctions
  void init_frame_ID(Cloud* cloud, int ID);
  void init_frame_ts(Subset* subset);
  void init_frame_chain(Frame* frame, Frame* frame_m1, Frame* frame_m2);

private:
  SLAM_map* mapManager;
  Scene* sceneManager;
};

#endif
