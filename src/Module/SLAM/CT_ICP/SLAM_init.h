#ifndef SLAM_INIT_H
#define SLAM_INIT_H

#include "../../../common.h"

class Slam;
class Scene;


class SLAM_init
{
public:
  //Constructor / Destructor
  SLAM_init(Slam* slam);
  ~SLAM_init();

public:
  //Main functions
  void update_configuration();
  void compute_initialization(Cloud* cloud, int subset_ID);

  //Subfunctions
  void init_frame_ID(Cloud* cloud, int ID);
  void init_frame_ts(Subset* subset);
  void init_frame_chain(Frame* frame, Frame* frame_m1, Frame* frame_m2);

  inline int get_frame_ID(){return map_frame_ID;}
  inline int get_ID_cloud(){return ID_cloud;}
  inline void set_frame_ID(int value){this->map_frame_ID = value;}

private:
  Scene* sceneManager;

  int map_frame_ID;
  int ID_cloud;
};

#endif
