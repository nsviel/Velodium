#ifndef PLAYER_ONLINE_H
#define PLAYER_ONLINE_H

#include "../../common.h"

class CT_ICP;
class Heatmap;
class Camera;
class Engine;
class Filter;
class Dimension;


class Player_online
{
public:
  //Constructor / Destructor
  Player_online(Engine* engineManager);
  ~Player_online();

public:
  void compute_onlineOpe(Cloud* cloud, int i);

  void camera_followUp(Cloud* cloud, int i);
  void camera_position(Subset* subset);
  void camera_orientation(Subset* subset);
  void save_image();

  inline bool* get_with_camera_follow(){return &with_camera_follow;}
  inline bool* get_with_slam(){return &with_slam;}
  inline bool* get_with_cylinder_filter(){return &with_cylinder_cleaning;}
  inline bool* get_with_heatmap(){return &with_heatmap;}
  inline bool* get_with_heatmap_rltHeight(){return &with_heatmap_rltHeight;}
  inline bool* get_with_save_image(){return &with_save_image;}
  inline bool* get_with_online(){return &with_online;}
  inline vec2* get_heatmap_height_range(){return &HM_height_range;}

private:
  Heatmap* heatmapManager;
  Camera* cameraManager;
  CT_ICP* cticpManager;
  Filter* filterManager;
  Dimension* dimManager;

  vec2 HM_height_range;
  vec2 camera_moved_trans;
  float camera_moved_rotat;
  float camera_distFromLidarPos;

  bool with_online;
  bool with_camera_follow;
  bool with_slam;
  bool with_camera_top;
  bool with_camera_root;
  bool with_cylinder_cleaning;
  bool with_heatmap;
  bool with_heatmap_rltHeight;
  bool with_save_image;
};

#endif
