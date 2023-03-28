#ifndef CAMERA_H
#define CAMERA_H

#include "struct_viewport.h"

#include "../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class Viewport;
class Shader;


class Camera
{
public:
  Camera(Node_engine* node);
  ~Camera();

public:
  //MVP matrices
  mat4 compute_cam_view();
  mat4 compute_cam_default();
  mat4 compute_cam_arcball();
  mat4 compute_cam_proj();
  mat4 compute_cam_mvp();
  mat4 compute_cam_world_pose();

  //Zoom
  void compute_zoom_optic(float value);
  void compute_zoom_position(float value);

  //Input commands
  void input_cam_mouse();
  void input_cam_mouse_default();
  void input_cam_mouse_arcball();

  //Camera mode
  void set_mode_projection(int value);
  void set_mode_angle(int value);
  void set_mode_view(int value);

  //Arcball function
  vec2 arcball_mouse_angle();
  void arcball_viewport_angle(vec2 angle);

  inline void set_desiredViewMatrix(mat4 value){viewport->cam_pose_mat = value;}
  inline void set_desiredPoseON(bool value){viewport->cam_pose = value;}
  inline void set_cameraFOV(float value){viewport->fov = value;}
  inline void set_cameraSpeed(float value){viewport->speed_move = value;}
  inline void set_cameraPos(vec3 value){viewport->cam_P = value;}
  inline void set_camForward(vec3 value){viewport->cam_F = value;}
  inline void set_angle_azimuth(float value){viewport->angle_azimuth = value;}

  inline mat4 get_cam_view(){return compute_cam_view();}
  inline mat4 get_cam_proj(){return compute_cam_proj();}
  inline mat4 get_mvpMatrix(){return compute_cam_mvp();}
  inline float* get_angle_azimuth(){return &viewport->angle_azimuth;}
  inline float get_angle_elevati(){return viewport->angle_elevation;}
  inline float get_fov(){return viewport->fov;}
  inline vec3 get_cam_P(){return viewport->cam_P;}
  inline vec3 get_cam_target(){return viewport->cam_P + viewport->cam_F;}
  inline vec3 get_cam_F(){return viewport->cam_F;}
  inline vec3 get_cam_U(){return viewport->cam_U;}
  inline vec3 get_cam_R(){return viewport->cam_R;}
  inline vec3* get_cam_COM(){return &viewport->cam_COM;}
  inline vec3* get_cam_PPtr(){return &viewport->cam_P;}
  inline Viewport_obj* get_current_viewport(){return viewport;}
  inline bool is_cameraMovON(){return viewport->cam_move;}

private:
  Configuration* configManager;
  Dimension* dimManager;
  Viewport *viewportManager;
  Shader* shaderManager;

  Viewport_obj* viewport;
  vec2 mouse_pose_old;
  int nb_viewport;
};

#endif
