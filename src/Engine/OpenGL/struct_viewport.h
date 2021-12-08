#ifndef STRUCT_VIEWPORT_H
#define STRUCT_VIEWPORT_H

#include <glm/glm.hpp>


struct Viewport_obj{
  glm::vec2 pos;
  glm::vec2 dim;

  glm::vec3 cam_F;
  glm::vec3 cam_R;
  glm::vec3 cam_U;
  glm::vec3 cam_pos;

  float angle_azimuth;
  float angle_elevati;

  float fov;
  float cam_speed;
  float zoom_UpView;

  bool proj_persp;
  bool proj_ortho;

  bool upView;
  bool sideView;

  bool cameraMovON;
  bool desiredPose;
  glm::mat4 dviewMat;
};

#endif
