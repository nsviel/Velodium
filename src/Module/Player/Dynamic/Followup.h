#ifndef FOLLOWUP_H
#define FOLLOWUP_H

#include "../../../common.h"

class Scene;
class Camera;
class Player_node;


class Followup
{
public:
  //Constructor / Destructor
  Followup(Player_node* node);
  ~Followup();

public:
  //Main function
  void camera_followUp(Cloud* cloud, int i);

  //Subfunctions
  void camera_displacment(Cloud* cloud, int ID_subset);
  vec3 camera_payload(Cloud* cloud, int ID_subset);
  void camera_position(Subset* subset, vec3 E);
  void camera_orientation(Subset* subset, vec3 E);
  void camera_reset();

  inline bool* get_with_camera_follow(){return &with_camera_follow;}
  inline bool* get_with_camera_absolute(){return &with_camera_absolute;}

private:
  Scene* sceneManager;
  Camera* cameraManager;

  vec2 camera_moved_trans;
  int camera_nb_pose;
  float camera_moved_rotat;
  float camera_distFromPos;

  bool with_camera_absolute;
  bool with_camera_follow;
  bool with_camera_top;
  bool with_camera_root;
};

#endif
