#ifndef CAMERA_H
#define CAMERA_H

class Dimension;

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

  //Accessors
  inline void set_fov(float value){this->fov = value;}
  inline void set_desiredViewMatrix(mat4 value){this->dviewMat = value;}
  inline void set_camPos(vec3 value){this->cam_pos = value;}
  inline void set_desiredPoseON(bool value){this->desiredPose = value;}
  inline void set_cameraMovON(bool value){this->cameraMovON = value;}
  inline void set_cameraSpeed(float value){this->cam_speed = value;}

  //Getters
  inline mat4 get_viewMat(){return compute_viewMat();}
  inline mat4 get_projMat(){return compute_projMat();}
  inline mat4 get_mvpMatrix(){return compute_mvpMatrix();}
  inline float get_horizAngle(){return horizAngle;}
  inline float get_vertiAngle(){return vertiAngle;}
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

  float horizAngle, vertiAngle;
  float fov, cam_speed;
  float deltaTime;
  float zoom_UpView;
  bool perspeView, orthoView;
  bool upView, sideView;
  bool cameraMovON;
  bool desiredPose;
  mat4 dviewMat;
  vec3 cam_F, cam_R, cam_U;
  vec3 cam_pos;
};

#endif
