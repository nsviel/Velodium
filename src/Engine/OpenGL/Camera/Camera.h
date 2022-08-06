#ifndef CAMERA_H
#define CAMERA_H

#include "struct_viewport.h"

#include "../../../common.h"

class Dimension;
class Configuration;


class Camera
{
public:
  Camera(Dimension* dim);
  ~Camera();

public:
  //Viewport stuff
  void viewport_init();
  void viewport_update(int ID);
  void viewport_reset();

  //MVP matrices
  mat4 compute_cam_view();
  mat4 compute_cam_proj();
  mat4 compute_cam_mvp();
  mat4 compute_cam_world_pose();

  //Functions
  void compute_view_arcball();
  void compute_zoom_optic(float value);
  void compute_zoom_position(float value);

  //Input commands
  void input_cam_mouse();
  void input_set_projection(int value);
  void input_set_view(int value);

  //Accessors
  inline void set_desiredViewMatrix(mat4 value){view_main.cam_pose_mat = value;}
  inline void set_desiredPoseON(bool value){view_main.cam_pose = value;}

  inline void set_cameraFOV(float value){view_main.fov = value;}
  inline void set_cameraSpeed(float value){view_main.speed_move = value;}
  inline void set_cameraPos(vec3 value){view_main.cam_P = value;}
  inline void set_camForward(vec3 value){view_main.cam_F = value;}
  inline void set_angle_azimuth(float value){view_main.angle_azimuth = value;}

  //Getters
  inline mat4 get_viewMat(){return compute_cam_view();}
  inline mat4 get_projMat(){return compute_cam_proj();}
  inline mat4 get_mvpMatrix(){return compute_cam_mvp();}
  inline float* get_angle_azimuth(){return &view_main.angle_azimuth;}
  inline float get_angle_elevati(){return view_main.angle_elevation;}
  inline float get_fov(){return view_main.fov;}
  inline vec3 get_camPos(){return view_main.cam_P;}
  inline vec3 get_camTarget(){return view_main.cam_P + view_main.cam_F;}
  inline vec3 get_camForward(){return view_main.cam_F;}
  inline vec3 get_camUp(){return view_main.cam_U;}
  inline vec3 get_camRight(){return view_main.cam_R;}
  inline vec3* get_camPosPtr(){return &view_main.cam_P;}
  inline int get_number_viewport(){return nb_viewport;}
  inline Viewport_obj* get_current_viewport(){return viewport;}

  inline bool is_cameraMovON(){return view_main.cam_move;}

private:
  Configuration* configManager;
  Dimension* dimManager;

  Viewport_obj view_main;
  Viewport_obj view_map;
  Viewport_obj* viewport;

  int nb_viewport;
};

#endif
