#ifndef CAMERA_H
#define CAMERA_H

class Dimension;

#include "struct_viewport.h"

#include "../../common.h"

class Camera
{
public:
  Camera(Dimension* dim);
  ~Camera();

public:
  //MVP matrices
  mat4 compute_viewMat();
  mat4 compute_projMat();
  mat4 compute_mvpMatrix();
  mat4 compute_worldPose();
  void compute_arcballRotation();

  //Inuts commands
  void input_cameraMouseCommands();
  void input_cameraKeyCommands();
  void input_projView(int value);

  //Functions
  void compute_opticalZoom(float value);
  void compute_positionalZoom(float value);
  void update_viewport();
  void update_viewport(int loop_cpt);

  //Accessors
  inline void set_desiredViewMatrix(mat4 value){this->dviewMat = value;}
  inline void set_desiredPoseON(bool value){this->desiredPose = value;}

  inline void set_cameraFOV(float value){this->fov = value;}
  inline void set_cameraMovON(bool value){this->cameraMovON = value;}
  inline void set_cameraSpeed(float value){this->cam_speed = value;}
  inline void set_cameraPos(vec3 value){this->cam_pos = value;}

  //Getters
  inline mat4 get_viewMat(){return compute_viewMat();}
  inline mat4 get_projMat(){return compute_projMat();}
  inline mat4 get_mvpMatrix(){return compute_mvpMatrix();}
  inline float get_angle_azimuth(){return angle_azimuth;}
  inline float get_angle_elevati(){return angle_elevati;}
  inline float get_fov(){return fov;}
  inline vec3 get_camPos(){return cam_pos;}
  inline vec3 get_camTarget(){return cam_pos + cam_F;}
  inline vec3 get_camForward(){return cam_F;}
  inline vec3 get_camUp(){return cam_U;}
  inline vec3 get_camRight(){return cam_R;}
  inline vec3* get_camPosPtr(){return &cam_pos;}

  inline bool is_cameraMovON(){return cameraMovON;}

private:
  Dimension* dimManager;
  Viewport_obj viewMain;
  Viewport_obj viewMap;

  vec2 pos;
  vec2 dim;

  vec3 cam_F;
  vec3 cam_R;
  vec3 cam_U;
  vec3 cam_pos;

  float angle_azimuth;
  float angle_elevati;
  float angle_elevati_old;

  float fov;
  float cam_speed;
  float zoom_topView;

  bool proj_persp;
  bool proj_ortho;
  bool topView;
  bool sideView;

  bool cameraMovON;
  bool desiredPose;
  mat4 dviewMat;
};

#endif
