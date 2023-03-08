#include "struct_viewport.h"


Viewport_obj::Viewport_obj(){
  //---------------------------

  this->pos = glm::vec2(0.0f);
  this->dim = glm::vec2(0.0f);

  this->cam_F = glm::vec3(0.0f);
  this->cam_R = glm::vec3(0.0f);
  this->cam_U = glm::vec3(0.0f);
  this->cam_P = glm::vec3(0.0f);
  this->cam_COM = glm::vec3(0.0f);

  this->angle_azimuth = 0;
  this->angle_elevation = 0;
  this->fov = 0;
  this->speed_mouse = 0;
  this->speed_move = 0;
  this->zoom = 0;
  this->clip_near = 0;
  this->clip_far = 0;

  this->projection = "";
  this->view = "";
  this->mode = "";

  this->cam_move = false;
  this->cam_pose = false;
  this->cam_pose_mat = glm::mat4(1.0f);

  //---------------------------
}
