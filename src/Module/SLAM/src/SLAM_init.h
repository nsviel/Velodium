#ifndef SLAM_INIT_H
#define SLAM_INIT_H

#include "../../../common.h"

class SLAM;
class SLAM_map;
class Scene;


class SLAM_init
{
public:
  //Constructor / Destructor
  SLAM_init(SLAM* slam);
  ~SLAM_init();

public:
  //Main functions
  void compute_initialization(Cloud* cloud, int subset_ID);

  //Subfunctions
  void init_frame_ID(Cloud* cloud, int subset_ID);
  void init_frame_ts(Cloud* cloud, int subset_ID);
  void init_frame_chain(Cloud* cloud, int subset_ID);

private:
  SLAM_map* slam_map;
  Scene* sceneManager;
};

#endif
