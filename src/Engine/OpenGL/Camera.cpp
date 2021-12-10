#include "Camera.h"



#include "../Configuration/Dimension.h"

#include <glm/gtc/matrix_transform.hpp>


//Constructor / Destructor
Camera::Camera(Dimension* dimension){
  this->dimManager = dimension;
  //---------------------------

  this->viewport_init();

  //---------------------------
}
Camera::~Camera(){}

//Viewport stuff
void Camera::viewport_init(){
  //---------------------------

  this->nb_viewport = 2;
  this->view = &viewMain;

  //Main viewport
  viewMain.cam_speed = configuration.CAM_MoveSpeed;
  viewMain.cam_P = vec3(configuration.CAM_InitialPos, configuration.CAM_InitialPos, configuration.CAM_InitialPos);
  viewMain.angle_azimuth = M_PI + M_PI/4;// Initial horizontal angle
  viewMain.angle_elevati = - M_PI/6;// Initial vertical angle
  viewMain.fov = configuration.CAM_FOV;
  viewMain.cameraMovON = false;
  viewMain.topView = false;
  viewMain.proj_persp = true;
  viewMain.proj_ortho = false;
  viewMain.desiredPose = false;
  viewMain.sideView = false;
  viewMain.zoom_topView = 0;

  //Map viewport
  viewMap.pos = vec2(800,400);
  viewMap.dim = vec2(100,100);
  viewMap.cam_P = vec3(0, 0, 0);
  viewMap.angle_azimuth = 0;
  viewMap.angle_elevati = 0;
  viewMap.fov = configuration.CAM_FOV;
  viewMap.proj_persp = false;
  viewMap.proj_ortho = true;
  viewMap.topView = true;
  viewMap.sideView = false;
  viewMap.zoom_topView = 0;

  //---------------------------
}
void Camera::viewport_update(int ID){
  //---------------------------

  //Main viewport
  if(ID == 0){
    dimManager->update_window_dim();
    view->dim = dimManager->get_glDim();
    view->pos = dimManager->get_glPos();

    glViewport(view->pos[0], view->pos[1], view->dim[0], view->dim[1]);
  }
  //Map viewport
  else if(ID == 1){
    glViewport(viewMap.pos[0], viewMap.pos[1], viewMap.dim[0], viewMap.dim[1]);
  }

  //---------------------------
}

//MVP Matrix
mat4 Camera::compute_viewMat(){
  mat4 viewMat;
  //---------------------------

  //Rigth camera
  view->cam_R = normalize(vec3(cos(view->angle_azimuth - M_PI/2.0f), sin(view->angle_azimuth - M_PI/2.0f), 0));

  //Forward and Up camera
  if(view->topView){
    view->angle_elevati_old = view->angle_elevati;
    view->angle_elevati = -M_PI/2.0f;
  }
  if(view->sideView) view->angle_elevati = 0.0f;

  view->cam_F = vec3(
    cos(view->angle_elevati) * cos(view->angle_azimuth),
    cos(view->angle_elevati) * sin(view->angle_azimuth),
    sin(view->angle_elevati));
  view->cam_U = normalize(cross(view->cam_R, view->cam_F));

  if(view->topView){
    view->angle_elevati = view->angle_elevati_old;
  }

  //Target camera
  vec3 cam_target = view->cam_P + view->cam_F;

  //View matrix computation
  viewMat = lookAt(view->cam_P, cam_target, view->cam_U);

  //OR Desired cam pose
  if(view->desiredPose){
    viewMat = view->dviewMat;
  }

  //---------------------------
  return viewMat;
}
mat4 Camera::compute_projMat(){
  mat4 projMat;
  //---------------------------

  //Compute projection matrix
  if(view->proj_persp){
    vec2 glDim = dimManager->get_glDim();
    projMat = perspective(radians(view->fov), (float)glDim.x / (float)glDim.y, configuration.CAM_NearClip, configuration.CAM_FarClip);
  }
  else if(view->proj_ortho){
    projMat = ortho(-5.f - view->zoom_topView, 5.f + view->zoom_topView, -5.f - view->zoom_topView, 5.f + view->zoom_topView, -1000.0f, 1000.0f);
  }
  else if(view->topView){
    projMat = ortho(-5.f - view->zoom_topView, 5.f + view->zoom_topView, -5.f - view->zoom_topView, 5.f + view->zoom_topView, -1000.0f, 1000.0f);
  }

  //---------------------------
  return projMat;
}
mat4 Camera::compute_mvpMatrix(){
  //---------------------------

  mat4 viewMat = compute_viewMat();
  mat4 projMat = compute_projMat();

  mat4 mvpMatrix = projMat * viewMat;

  //---------------------------
  return mvpMatrix;
}
mat4 Camera::compute_worldPose(){
  //---------------------------

  vec3 zaxis = normalize(view->cam_F);
  vec3 xaxis = normalize(cross(view->cam_U, zaxis));
  vec3 yaxis = cross(zaxis, xaxis);

  mat4 absPose(
         xaxis[0], yaxis[0], zaxis[0], view->cam_P[0],
         xaxis[1], yaxis[1], zaxis[1], view->cam_P[1],
         xaxis[2], yaxis[2], zaxis[2], view->cam_P[2],
           0,       0,       0,     1);

  //---------------------------
  return absPose;
}
void Camera::compute_arcballRotation(){
  //---------------------------

  mat4 arc_Rx(1.0);
  mat4 arc_Rz(1.0);

  //---------------------------
}

//Inputs
void Camera::input_cameraMouseCommands(){
  //---------------------------

  if(view->cameraMovON){
    //Cursor movement
    vec2 curPos = dimManager->get_cursorPos();
    dimManager->set_cursorPos(dimManager->get_glMiddle());

    // Compute new orientation
    vec2 glMid = dimManager->get_glMiddle();
    view->angle_azimuth += configuration.CAM_MouseSpeed * float(glMid.x - curPos.x);
    view->angle_elevati += configuration.CAM_MouseSpeed * float(glMid.y - curPos.y);

    //Limites of camera rotation
    if(view->angle_elevati > M_PI/2) view->angle_elevati = M_PI/2;
    if(view->angle_elevati < -M_PI/2) view->angle_elevati = -M_PI/2;
    if(view->angle_azimuth > M_PI*2) view->angle_azimuth = 0;
    if(view->angle_azimuth < -M_PI*2) view->angle_azimuth = 0;

    glfwSetInputMode(dimManager->get_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  }

  //---------------------------
}
void Camera::input_projView(int projView){
  view->proj_persp = false;
  view->proj_ortho = false;
  view->topView = false;
  view->sideView = false;
  //---------------------------

  switch(projView){
    case 0:{ //Perspective
      view->proj_persp = true;
      break;
    }
    case 1:{ //Orthographic
      view->proj_ortho = true;
      break;
    }
    case 2:{ //Ortho up
      view->topView = true;
      break;
    }
    case 3:{ //Ortho side
      view->sideView = true;
      break;
    }
  }

  //---------------------------
}

//Functions
void Camera::compute_opticalZoom(float yoffset){
  GLFWwindow* window = glfwGetCurrentContext();
  //---------------------------

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
    //Perspective zoom
    if(view->fov >= 1.0f && view->fov <= configuration.CAM_FOV) view->fov -= yoffset;
    if(view->fov <= 1.0f) view->fov = 1.0f;
    if(view->fov >= configuration.CAM_FOV) view->fov = configuration.CAM_FOV;

    //Ortho zoom
    view->zoom_topView -= yoffset * 0.1;
  }

  //---------------------------
}
void Camera::compute_positionalZoom(float yoffset){
  GLFWwindow* window = glfwGetCurrentContext();
  //---------------------------

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
    vec3 cam_forwardMove = view->cam_F * yoffset * view->cam_speed * vec3(0.1,0.1,0.1);
    view->cam_P += cam_forwardMove;
  }

  //---------------------------
}
