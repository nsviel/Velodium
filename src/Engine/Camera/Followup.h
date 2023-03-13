#ifndef FOLLOWUP_H
#define FOLLOWUP_H

#include "../../common.h"

class Node_engine;
class Camera;
class Player_node;
class Configuration;


class Followup
{
public:
  //Constructor / Destructor
  Followup(Node_engine* node);
  ~Followup();

public:
  //Main function
  void update_configuration();
  void camera_followup(Collection* collection, int i);
  void camera_mode(string mode);

  //Subfunctions
  vec3 camera_payload(Collection* collection, int ID_object);
  void camera_position(Cloud* cloud, vec3 E);
  void camera_orientation(Cloud* cloud, vec3 E);
  void camera_reset();

  inline bool* get_with_camera_follow(){return &with_camera_follow;}
  inline bool* get_with_camera_absolute(){return &with_camera_absolute;}
  inline bool* get_with_camera_top(){return &with_camera_top;}

private:
  Camera* cameraManager;
  Configuration* configManager;

  vec3 camera_moved_trans;
  float camera_distFromPos;
  float camera_top_z;
  float camera_oblique_z;
  int camera_nb_pose;

  bool with_camera_absolute;
  bool with_camera_follow;
  bool with_camera_top;
};

#endif
