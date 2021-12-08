#include "Camera.h"



#include "../Configuration/Dimension.h"

#include <glm/gtc/matrix_transform.hpp>


//Constructor / Destructor
Camera::Camera(Dimension* dimension){
  this->dimManager = dimension;
  //---------------------------

  viewMain.cam_speed = configuration.CAM_MoveSpeed;
  this->cam_pos = vec3(configuration.CAM_InitialPos, configuration.CAM_InitialPos, configuration.CAM_InitialPos);
  this->angle_azimuth = M_PI + M_PI/4;// Initial horizontal angle
  this->angle_elevati = - M_PI/6;// Initial vertical angle
  this->fov = configuration.CAM_FOV;
  this->cameraMovON = false;
  this->topView = false;
  this->proj_persp = true;
  this->proj_ortho = false;
  this->desiredPose = false;
  this->sideView = false;
  this->zoom_topView = 0;



  //---------------------------
}
Camera::~Camera(){}

//MVP Matrix
mat4 Camera::compute_viewMat(){
  mat4 viewMat;
  //---------------------------

  //Rigth camera
  cam_R = normalize(vec3(cos(angle_azimuth - M_PI/2.0f), sin(angle_azimuth - M_PI/2.0f), 0));

  //Forward and Up camera
  if(topView){
    angle_elevati_old = angle_elevati;
    angle_elevati = -M_PI/2.0f;
  }
  if(sideView) angle_elevati = 0.0f;

  cam_F = vec3(
    cos(angle_elevati) * cos(angle_azimuth),
    cos(angle_elevati) * sin(angle_azimuth),
    sin(angle_elevati));
  cam_U = normalize(cross(cam_R, cam_F));

  if(topView){
    angle_elevati = angle_elevati_old;
  }

  //Target camera
  vec3 cam_target = cam_pos + cam_F;

  //View matrix computation
  viewMat = lookAt(cam_pos, cam_target, cam_U);

  //OR Desired cam pose
  if(desiredPose){
    viewMat = dviewMat;
  }

  //---------------------------
  return viewMat;
}
mat4 Camera::compute_projMat(){
  mat4 projMat;
  //---------------------------

  //Compute projection matrix
  if(proj_persp){
    vec2 glDim = dimManager->get_glDim();
    projMat = perspective(radians(fov), (float)glDim.x / (float)glDim.y, configuration.CAM_NearClip, configuration.CAM_FarClip);
  }
  else if(proj_ortho){
    projMat = ortho(-5.f - zoom_topView, 5.f + zoom_topView, -5.f - zoom_topView, 5.f + zoom_topView, -1000.0f, 1000.0f);
  }
  else if(topView){
    projMat = ortho(-5.f - zoom_topView, 5.f + zoom_topView, -5.f - zoom_topView, 5.f + zoom_topView, -1000.0f, 1000.0f);
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

  vec3 zaxis = normalize(cam_F);
  vec3 xaxis = normalize(cross(cam_U, zaxis));
  vec3 yaxis = cross(zaxis, xaxis);

  mat4 absPose(
         xaxis[0], yaxis[0], zaxis[0], cam_pos[0],
         xaxis[1], yaxis[1], zaxis[1], cam_pos[1],
         xaxis[2], yaxis[2], zaxis[2], cam_pos[2],
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
void Camera::input_cameraKeyCommands(){
  //---------------------------

  if(cameraMovON){
    float delta = 0.016;
    float CAMspeed = cam_speed * delta;
    float fastSpeed = cam_speed * delta * 10;
    //--------------------

    //Keystrocks
    GLFWwindow* window = glfwGetCurrentContext();
    if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS){
      CAMspeed = fastSpeed;
    }
    if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS || glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){
      cam_pos += cam_F * CAMspeed;
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN)==GLFW_PRESS || glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){
      cam_pos -= cam_F * CAMspeed;
    }
    if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS || glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){
      cam_pos += cam_R * CAMspeed;
    }
    if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS || glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){
      cam_pos -= cam_R * CAMspeed;
    }

    //topView keystrocks
    if(topView){
      if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){
        cam_pos += cam_U * CAMspeed;
      }
      if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){
        cam_pos -= cam_U * CAMspeed;
      }
    }
  }

  //---------------------------
}
void Camera::input_cameraMouseCommands(){
  //---------------------------

  if(cameraMovON){
    //Cursor movement
    vec2 curPos = dimManager->get_cursorPos();
    dimManager->set_cursorPos(dimManager->get_glMiddle());

    // Compute new orientation
    vec2 glMid = dimManager->get_glMiddle();
    angle_azimuth += configuration.CAM_MouseSpeed * float(glMid.x - curPos.x);
    angle_elevati += configuration.CAM_MouseSpeed * float(glMid.y - curPos.y);

    //Limites of camera rotation
    if(angle_elevati > 1.57) angle_elevati = 1.57;
    if(angle_elevati < -1.57) angle_elevati = -1.57;
    if(angle_azimuth > 2*M_PI) angle_azimuth = 0;
    if(angle_azimuth < 2*(-M_PI)) angle_azimuth = 0;
  }

  //---------------------------
}
void Camera::input_projView(int projView){
  this->proj_persp = false;
  this->proj_ortho = false;
  this->topView = false;
  this->sideView = false;
  //---------------------------

  switch(projView){
    case 0:{ //Perspective
      this->proj_persp = true;
      break;
    }
    case 1:{ //Orthographic
      this->proj_ortho = true;
      break;
    }
    case 2:{ //Ortho up
      this->topView = true;
      break;
    }
    case 3:{ //Ortho side
      this->sideView = true;
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
    if(fov >= 1.0f && fov <= configuration.CAM_FOV) fov -= yoffset;
    if(fov <= 1.0f) fov = 1.0f;
    if(fov >= configuration.CAM_FOV) fov = configuration.CAM_FOV;

    //Ortho zoom
    zoom_topView -= yoffset * 0.1;
  }

  //---------------------------
}
void Camera::compute_positionalZoom(float yoffset){
  GLFWwindow* window = glfwGetCurrentContext();
  //---------------------------

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
    vec3 cam_forwardMove = cam_F * yoffset * cam_speed * vec3(0.1,0.1,0.1);
    cam_pos += cam_forwardMove;
  }

  //---------------------------
}
