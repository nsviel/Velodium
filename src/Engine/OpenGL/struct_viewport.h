#ifndef STRUCT_VIEWPORT_H
#define STRUCT_VIEWPORT_H

#include <glm/glm.hpp>


struct Viewport_obj{
  glm::vec2 pos;
  glm::vec2 dim;

  glm::vec3 cam_F; // Front
  glm::vec3 cam_R; // Right
  glm::vec3 cam_U; // Up
  glm::vec3 cam_P; // Position

  float angle_azimuth;
  float angle_elevati;
  float angle_elevati_old;

  float fov;
  float speed_mouse;
  float speed_move;
  float zoom_topView;
  float clip_near;
  float clip_far;

  bool proj_persp;
  bool proj_ortho;

  bool topView;
  bool sideView;

  bool cameraMovON;
  bool desiredPose;
  glm::mat4 dviewMat;
};

#endif
