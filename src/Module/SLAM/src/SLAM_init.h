#ifndef SLAM_INIT_H
#define SLAM_INIT_H

#include "../Base/common.h"

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
  void compute_initialization(Collection* collection, int subset_ID);

  //Subfunctions
  void init_frame_ID(Collection* collection, int subset_ID);
  void init_frame_ts(Collection* collection, int subset_ID);
  void init_frame_chain(Collection* collection, int subset_ID);

private:
  SLAM_map* slam_map;
  Scene* sceneManager;
};

#endif
