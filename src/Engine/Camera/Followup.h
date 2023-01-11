#ifndef FOLLOWUP_H
#define FOLLOWUP_H

#include "../../common.h"

class Node_engine;
class Scene;
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
  void camera_followup(Cloud* cloud, int i);
  void camera_mode(string mode);

  //Subfunctions
  vec3 camera_payload(Cloud* cloud, int ID_subset);
  void camera_position(Subset* subset, vec3 E);
  void camera_orientation(Subset* subset, vec3 E);
  void camera_reset();

  inline bool* get_with_camera_follow(){return &with_camera_follow;}
  inline bool* get_with_camera_absolute(){return &with_camera_absolute;}
  inline bool* get_with_camera_top(){return &with_camera_top;}

private:
  Scene* sceneManager;
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
