#ifndef OnlinePlayer_H
#define OnlinePlayer_H

#include "../../common.h"

class CT_ICP;
class Heatmap;
class Camera;


class OnlinePlayer
{
public:
  //Constructor / Destructor
  OnlinePlayer(Camera* camManager);
  ~OnlinePlayer();

public:
  void make_thing(Subset* subset);
  void camera_followUp(Subset* subset);

  inline bool* get_with_camera_follow(){return &with_camera_follow;}
  inline bool* get_with_slam(){return &with_slam;}
  inline bool* get_with_cylinder_filter(){return &with_cylinder_cleaning;}
  inline bool* get_with_heatmap(){return &with_heatmap;}

private:
  Heatmap* heatmapManager;
  Camera* cameraManager;
  CT_ICP* slamManager;

  bool with_camera_follow;
  bool with_slam;
  bool with_camera_top;
  bool with_camera_root;
  bool with_cylinder_cleaning;
  bool with_heatmap;
};

#endif
