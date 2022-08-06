#ifndef STRUCT_VIEWPORT_H
#define STRUCT_VIEWPORT_H

#include <glm/glm.hpp>
#include <string>


struct Viewport_obj{
  glm::vec2 pos;
  glm::vec2 dim;

  glm::vec3 cam_F; // Front
  glm::vec3 cam_R; // Right
  glm::vec3 cam_U; // Up
  glm::vec3 cam_P; // Pose

  float angle_azimuth;
  float angle_elevation;

  float fov;
  float speed_mouse;
  float speed_move;
  float zoom;
  float clip_near;
  float clip_far;

  std::string projection;
  std::string view;

  bool cam_move;
  bool cam_pose;
  glm::mat4 cam_pose_mat;
};

#endif
