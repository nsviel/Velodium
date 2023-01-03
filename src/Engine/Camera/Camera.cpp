#include "Camera.h"
#include "Viewport.h"

#include "../Node_engine.h"
#include "../Core/Dimension.h"
#include "../Scene/Configuration.h"

#include <glm/gtc/matrix_transform.hpp>


//Constructor / Destructor
Camera::Camera(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();
  this->configManager = node_engine->get_configManager();
  this->viewportManager = node_engine->get_viewportManager();

  this->viewport = viewportManager->get_viewport_main();

  //---------------------------
}
Camera::~Camera(){}

//MVP Matrix
mat4 Camera::compute_cam_view(){
  mat4 viewMat;
  //---------------------------

  if(viewport->cam_pose){
    viewMat = viewport->cam_pose_mat;
  }else if(viewport->mode == "default"){
    viewMat = compute_cam_default();
  }else if(viewport->mode == "arcball"){
    viewMat = compute_cam_arcball();
  }

  //---------------------------
  return viewMat;
}
mat4 Camera::compute_cam_default(){
  mat4 viewMat;
  //---------------------------

  float azimuth = viewport->angle_azimuth;
  float elevation = viewport->angle_elevation;

  //Forward and Up camera
  if(viewport->view == "top"){
    elevation = -M_PI/2.0f;
  }

  //Compute camera
  viewport->cam_R = normalize(vec3(cos(azimuth - M_PI/2.0f), sin(azimuth - M_PI/2.0f), 0));
  viewport->cam_F = vec3(cos(elevation) * cos(azimuth), cos(elevation) * sin(azimuth), sin(elevation));
  viewport->cam_U = normalize(cross(viewport->cam_R, viewport->cam_F));
  vec3 cam_target = viewport->cam_P + viewport->cam_F;

  //Compute view matrix
  viewMat = lookAt(viewport->cam_P, cam_target, viewport->cam_U);

  //---------------------------
  return viewMat;
}
mat4 Camera::compute_cam_arcball(){
  mat4 viewMat;
  //---------------------------

  //Compute camera
  viewport->cam_F = viewport->cam_COM - viewport->cam_P;
  viewport->cam_U = normalize(cross(viewport->cam_R, viewport->cam_F));

  //Compute view matrix
  viewMat = lookAt(viewport->cam_P, viewport->cam_COM, viewport->cam_U);

  //---------------------------
  return viewMat;
}
mat4 Camera::compute_cam_proj(){
  mat4 projMat;
  //---------------------------

  //Compute projection matrix
  if(viewport->projection == "perspective"){
    vec2 glDim = dimManager->get_gl_dim();
    float znear = viewport->clip_near;
    float zfar = viewport->clip_far;
    float fov = radians(viewport->fov);
    projMat = perspective(fov, glDim.x / glDim.y, znear, zfar);
  }
  else if(viewport->projection == "orthographic"){
    float zoom = viewport->zoom;
    projMat = ortho(-5.f - zoom, 5.f + zoom, -5.f - zoom, 5.f + zoom, -1000.0f, 1000.0f);
  }

  //---------------------------
  return projMat;
}
mat4 Camera::compute_cam_mvp(){
  //---------------------------

  //mat4 viewMat = compute_cam_view();
  mat4 viewMat = compute_cam_view();
  mat4 projMat = compute_cam_proj();

  mat4 mvpMatrix = projMat * viewMat;

  //---------------------------
  return mvpMatrix;
}
mat4 Camera::compute_cam_world_pose(){
  //---------------------------

  vec3 zaxis = normalize(viewport->cam_F);
  vec3 xaxis = normalize(cross(viewport->cam_U, zaxis));
  vec3 yaxis = cross(zaxis, xaxis);

  mat4 absPose(
         xaxis[0], yaxis[0], zaxis[0], viewport->cam_P[0],
         xaxis[1], yaxis[1], zaxis[1], viewport->cam_P[1],
         xaxis[2], yaxis[2], zaxis[2], viewport->cam_P[2],
           0,       0,       0,     1);

  //---------------------------
  return absPose;
}

//Functions
void Camera::compute_zoom_optic(float yoffset){
  GLFWwindow* window = glfwGetCurrentContext();
  //---------------------------

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
    //Perspective zoom
    float camFOV = configManager->parse_json_f("camera", "fov");
    if(viewport->fov >= 1.0f && viewport->fov <= camFOV) viewport->fov -= yoffset;
    if(viewport->fov <= 1.0f) viewport->fov = 1.0f;
    if(viewport->fov >= camFOV) viewport->fov = camFOV;

    //Ortho zoom
    viewport->zoom -= yoffset * 0.1;
  }

  //---------------------------
}
void Camera::compute_zoom_position(float yoffset){
  GLFWwindow* window = glfwGetCurrentContext();
  //---------------------------

  if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
    //Perspective zoom
    vec3 cam_forwardMove = viewport->cam_F * yoffset * viewport->speed_move * vec3(0.1,0.1,0.1);
    viewport->cam_P += cam_forwardMove;

    //Ortho zoom
    viewport->zoom -= yoffset * 0.1;
  }

  //---------------------------
}

//Inputs
void Camera::input_cam_mouse(){
  if(viewport->cam_move){
    //---------------------------

    if(viewport->mode == "default"){
      this->input_cam_mouse_default();
    }else if(viewport->mode == "arcball"){
      this->input_cam_mouse_arcball();
    }

    //---------------------------
  }
}
void Camera::input_cam_mouse_default(){
  //---------------------------

  float& azimuth = viewport->angle_azimuth;
  float& elevation = viewport->angle_elevation;

  //Cursor movement
  vec2 mouse_pose = dimManager->get_mouse_pose();
  dimManager->set_mouse_pose(dimManager->get_gl_middle());

  // Compute new orientation
  vec2 gl_mid = dimManager->get_gl_middle();
  azimuth += viewport->speed_mouse * float(gl_mid.x - mouse_pose.x);
  elevation += viewport->speed_mouse * float(gl_mid.y - mouse_pose.y);

  //Limites of camera rotation
  if(elevation > M_PI/2) elevation = M_PI/2;
  if(elevation < -M_PI/2) elevation = -M_PI/2;
  if(azimuth > M_PI*2) azimuth = 0;
  if(azimuth < -M_PI*2) azimuth = 0;

  glfwSetInputMode(dimManager->get_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  //---------------------------
}
void Camera::input_cam_mouse_arcball(){
  //---------------------------

  vec2 angle = arcball_mouse_angle();
  this->arcball_viewport_angle(angle);

  //---------------------------
}

//Arcball function
vec2 Camera::arcball_mouse_angle(){
  //---------------------------

  vec2 mouse_pose = dimManager->get_mouse_pose();
  dimManager->set_mouse_pose(dimManager->get_gl_middle());
  vec2 gl_mid = dimManager->get_gl_middle();
  vec2 gl_dim = dimManager->get_gl_dim();

  // step 1 : Calculate the amount of rotation given the mouse movement.
  float deltaAngleX = (2 * M_PI / gl_dim.x); // a movement from left to right = 2*PI = 360 deg
  float deltaAngleY = (M_PI / gl_dim.y);  // a movement from top to bottom = PI = 180 deg
  float xAngle = float(gl_mid.x - mouse_pose.x) * deltaAngleX * 0.1;
  float yAngle = float(gl_mid.y - mouse_pose.y) * deltaAngleY * 0.1;
  vec2 angle = vec2(xAngle, yAngle);

  glfwSetInputMode(dimManager->get_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  //---------------------------
  return angle;
}
void Camera::arcball_viewport_angle(vec2 angle){
  //---------------------------

  // Get the homogenous position of the camera and pivot point
  vec4 cam_P (viewport->cam_P.x, viewport->cam_P.y, viewport->cam_P.z, 1);
  vec4 cam_COM (viewport->cam_COM.x, viewport->cam_COM.y, viewport->cam_COM.z, 1);
  vec4 cam_R (viewport->cam_R.x, viewport->cam_R.y, viewport->cam_R.z, 1);

  // step 2: Rotate the camera around the pivot point on the first axis.
  mat4x4 Rz(1.0f);
  Rz = glm::rotate(Rz, angle.x, vec3(0, 0, 1));
  cam_P  = (Rz * (cam_P - cam_COM)) + cam_COM;
  viewport->cam_R = Rz * cam_R;

  // step 3: Rotate the camera around the pivot point on the second axis.
  mat4x4 Rr(1.0f);
  Rr = glm::rotate(Rr, angle.y, viewport->cam_R);
  viewport->cam_P = (Rr * (cam_P - cam_COM)) + cam_COM;

  //---------------------------
}

//Camera mode
void Camera::set_mode_projection(int proj){
  //---------------------------

  switch(proj){
    case 0:{ //Perspective
      viewport->projection = "perspective";
      break;
    }
    case 1:{ //Orthographic
      viewport->projection = "orthographic";
      break;
    }
  }

  //---------------------------
}
void Camera::set_mode_angle(int view){
  //---------------------------

  switch(view){
    case 0:{ //Top
      viewport->view = "top";
      break;
    }
    case 1:{ //Oblique
      viewport->view = "oblique";
      break;
    }
  }

  //---------------------------
}
void Camera::set_mode_view(int mode){
  viewportManager->viewport_reset();
  //---------------------------

  switch(mode){
    case 0:{ //Default
      viewport->mode = "default";
      break;
    }
    case 1:{ //Arcball
      viewport->mode = "arcball";
      break;
    }
  }

  //---------------------------
}
