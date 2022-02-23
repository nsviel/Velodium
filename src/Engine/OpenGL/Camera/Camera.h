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
  mat4 compute_viewMat();
  mat4 compute_projMat();
  mat4 compute_mvpMatrix();
  mat4 compute_worldPose();
  void compute_arcballRotation();

  //Inuts commands
  void input_cameraMouseCommands();
  void input_projView(int value);

  //Functions
  void compute_opticalZoom(float value);
  void compute_positionalZoom(float value);

  //Accessors
  inline void set_desiredViewMatrix(mat4 value){viewMain.dviewMat = value;}
  inline void set_desiredPoseON(bool value){viewMain.desiredPose = value;}

  inline void set_cameraFOV(float value){viewMain.fov = value;}
  inline void set_cameraSpeed(float value){viewMain.speed_move = value;}
  inline void set_cameraPos(vec3 value){viewMain.cam_P = value;}
  inline void set_camForward(vec3 value){viewMain.cam_F = value;}
  inline void set_angle_azimuth(float value){viewMain.angle_azimuth = value;}

  //Getters
  inline mat4 get_viewMat(){return compute_viewMat();}
  inline mat4 get_projMat(){return compute_projMat();}
  inline mat4 get_mvpMatrix(){return compute_mvpMatrix();}
  inline float* get_angle_azimuth(){return &viewMain.angle_azimuth;}
  inline float get_angle_elevati(){return viewMain.angle_elevati;}
  inline float get_fov(){return viewMain.fov;}
  inline vec3 get_camPos(){return viewMain.cam_P;}
  inline vec3 get_camTarget(){return viewMain.cam_P + viewMain.cam_F;}
  inline vec3 get_camForward(){return viewMain.cam_F;}
  inline vec3 get_camUp(){return viewMain.cam_U;}
  inline vec3 get_camRight(){return viewMain.cam_R;}
  inline vec3* get_camPosPtr(){return &viewMain.cam_P;}
  inline int get_number_viewport(){return nb_viewport;}
  inline Viewport_obj* get_current_viewport(){return view;}

  inline bool is_cameraMovON(){return viewMain.cameraMovON;}

private:
  Configuration* configManager;
  Dimension* dimManager;

  Viewport_obj viewMain;
  Viewport_obj viewMap;
  Viewport_obj* view;

  int nb_viewport;
};

#endif
