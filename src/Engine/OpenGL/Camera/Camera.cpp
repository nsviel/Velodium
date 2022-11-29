#include "Camera.h"

#include "../../OpenGL/Dimension.h"
#include "../../Scene/Configuration.h"

#include <glm/gtc/matrix_transform.hpp>


//Constructor / Destructor
Camera::Camera(Dimension* dimension){
  this->dimManager = dimension;
  //---------------------------

  this->configManager = new Configuration();

  this->viewport_init();

  //---------------------------
}
Camera::~Camera(){
  //---------------------------

  delete configManager;

  //---------------------------
}

//Viewport stuff
void Camera::viewport_init(){
  //---------------------------

  this->nb_viewport = 2;
  this->viewport = &view_main;

  //Main viewport
  float camPos = configManager->parse_json_f("camera", "initial_pos");
  view_main.cam_P = vec3(camPos, camPos, camPos);
  view_main.speed_move = configManager->parse_json_f("camera", "speed_move");
  view_main.speed_mouse = configManager->parse_json_f("camera", "speed_mouse");
  view_main.angle_azimuth = M_PI + M_PI/4;// Initial horizontal angle
  view_main.angle_elevation = - M_PI/6;// Initial vertical angle
  view_main.fov = configManager->parse_json_f("camera", "fov");
  view_main.cam_move = false;
  view_main.cam_pose = false;
  view_main.view = "oblique";
  view_main.projection = "perspective";
  view_main.mode = "default";
  view_main.cam_COM = vec3(0, 0, 0);
  view_main.zoom = 0;
  view_main.clip_near = configManager->parse_json_f("camera", "clip_near");
  view_main.clip_far = configManager->parse_json_f("camera", "clip_far");

  //Map viewport
  view_map.pos = vec2(800,400);
  view_map.dim = vec2(100,100);
  view_map.cam_P = vec3(0, 0, 0);
  view_map.angle_azimuth = 0;
  view_map.angle_elevation = 0;
  view_map.fov = configManager->parse_json_f("camera", "fov");
  view_map.projection = "perspective";
  view_map.view = "oblique";
  view_map.zoom = 0;
  view_map.clip_near = configManager->parse_json_f("camera", "clip_near");
  view_map.clip_far = configManager->parse_json_f("camera", "clip_far");

  //---------------------------
}
void Camera::viewport_update(int ID){
  //---------------------------

  //Main viewport
  if(ID == 0){
    viewport->dim = dimManager->get_gl_dim();
    viewport->pos = dimManager->get_gl_pos();

    vec2 gl_dim = dimManager->get_gl_dim();
    glViewport(0, 0, gl_dim[0], gl_dim[1]);
  }
  //Map viewport
  else if(ID == 1){
    glViewport(view_map.pos[0], view_map.pos[1], view_map.dim[0], view_map.dim[1]);
  }

  //---------------------------
}
void Camera::viewport_reset(){
  //---------------------------

  float camPos = configManager->parse_json_f("camera", "initial_pos");
  view_main.cam_P = vec3(camPos, camPos, camPos);
  view_main.angle_azimuth = M_PI + M_PI/4;// Initial horizontal angle
  view_main.angle_elevation = - M_PI/6;// Initial vertical angle

  //---------------------------
}

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

  float azimuth = viewport->angle_azimuth;
  float elevation = viewport->angle_elevation;

  //Compute camera
  viewport->cam_R = normalize(vec3(cos(azimuth - M_PI/2.0f), sin(azimuth - M_PI/2.0f), 0));
  viewport->cam_F = -viewport->cam_P;
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

  vec2 mouse_pose = dimManager->get_mouse_pose();
  dimManager->set_mouse_pose(dimManager->get_gl_middle());
  vec2 gl_mid = dimManager->get_gl_middle();
  vec2 gl_dim = dimManager->get_gl_dim();

  // step 1 : Calculate the amount of rotation given the mouse movement.
  float deltaAngleX = (2 * M_PI / gl_dim.x); // a movement from left to right = 2*PI = 360 deg
  float deltaAngleY = (M_PI / gl_dim.y);  // a movement from top to bottom = PI = 180 deg
  float xAngle = float(gl_mid.x - mouse_pose.x) * deltaAngleX * 0.1;
  float yAngle = float(gl_mid.y - mouse_pose.y) * deltaAngleY * 0.1;

  // Get the homogenous position of the camera and pivot point
  glm::vec4 position (view_main.cam_P.x, view_main.cam_P.y, view_main.cam_P.z, 1);
  glm::vec4 pivot (viewport->cam_COM.x, viewport->cam_COM.y, viewport->cam_COM.z, 1);

  // step 2: Rotate the camera around the pivot point on the first axis.
  glm::mat4x4 rotationMatrixX(1.0f);
  rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, view_main.cam_U);
  position = (rotationMatrixX * (position - pivot)) + pivot;

  // step 3: Rotate the camera around the pivot point on the second axis.
  glm::mat4x4 rotationMatrixY(1.0f);
  rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, view_main.cam_R);
  glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

  view_main.cam_P = finalPosition;

  // Compute new orientation
  vec3 C = view_main.cam_F;
  vec3 D = view_main.cam_P;
  float angle_x = atan(C.y, C.x) - atan(D.y, D.x);
  angle_x = angle_x + M_PI;
  float& azimuth = viewport->angle_azimuth;
  azimuth -= angle_x;

  glfwSetInputMode(dimManager->get_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  //---------------------------
}
void Camera::input_set_projection(int proj){
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
void Camera::input_set_view(int view){
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
void Camera::input_set_mode(int mode){
  this->viewport_reset();
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
