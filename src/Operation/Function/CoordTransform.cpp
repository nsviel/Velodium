#include "CoordTransform.h"

#include "../Node_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/OpenGL/Camera/Camera.h"
#include "../../Engine/OpenGL/Dimension.h"

#include <glm/gtc/matrix_transform.hpp>


//Constructor / Destructor
CoordTransform::CoordTransform(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->cameraManager = node_engine->get_cameraManager();
  this->dimManager = node_engine->get_dimManager();

  //---------------------------
}
CoordTransform::~CoordTransform(){}

vec2 CoordTransform::WorldToScreen(vec3 point){
  mat4 projMat = cameraManager->get_projMat();
  mat4 viewMat = cameraManager->get_viewMat();
  vec2 glPos = dimManager->get_gl_pos();
  vec2 glDim = dimManager->get_gl_dim();
  vec2 pt_out;
  //---------------------------

  vec4 viewport(0, 0, glDim.x, glDim.y);

  vec3 projected = glm::project(point, viewMat, projMat, viewport);
  pt_out.x = projected.x;
  pt_out.y = glDim.y - projected.y;

  //Check if point is inside the screen
  if(projected.z > 1.0f || pt_out.x < 0 || pt_out.x > glDim.x || pt_out.y < 0 || pt_out.y > glDim.y){
    pt_out = vec2(-1.0f, -1.0f);
  }

  //---------------------------
  return pt_out;
}
vec3 CoordTransform::ScreenToWorld(vec2 cursorPos){
  vec2 glDim = dimManager->get_gl_dim();
  float gui_X = ImGui::GetWindowSize().x;
  float gui_Y = ImGui::GetWindowSize().y;
  float gl_X = glDim.x;
  float gl_Y = glDim.y;
  //---------------------------

  //Get ray direction
  float x = (2.0f * cursorPos.x) / gl_X - 1.0f;
  float y = 1.0f - (2.0f * cursorPos.y) / gl_Y;
  float z = 1.0f;
  vec3 ray_nds = vec3(x, y, z);
  vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
  mat4 projMat = cameraManager->get_projMat();
  vec4 ray_eye = inverse(projMat) * ray_clip;
  ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
  mat4 viewMat = cameraManager->get_viewMat();
  vec4 ray_wor = inverse(viewMat) * ray_eye;
  vec3 ray_world = vec3(ray_wor);
  vec3 ray_dir = normalize(ray_world);

  vec3 cam_pos = cameraManager->get_camPos();
  vec3 cam_forw = cameraManager->get_camForward();
  vec3 frustum = cam_pos + vec3(cam_forw.x*0.5,cam_forw.y*0.5,cam_forw.z*0.5);

  vec3 D = ray_dir;
  vec3 N = cam_forw;
  vec3 X = cam_pos + D * dot(frustum - cam_pos, N) / dot(D, N);

  //---------------------------
  return X;
}
vec3 CoordTransform::CursorToGround(){
  vec2 cursorPos = dimManager->get_mouse_pose();
  vec2 glDim = dimManager->get_gl_dim();
  float gui_X = ImGui::GetWindowSize().x;
  float gui_Y = ImGui::GetWindowSize().y;
  float gl_X = glDim.x;
  float gl_Y = glDim.y;
  //---------------------------

  //Get ray direction
  float x = (2.0f * cursorPos.x) / gl_X - 1.0f;
  float y = 1.0f - (2.0f * cursorPos.y) / gl_Y;
  float z = 1.0f;
  vec3 ray_nds = vec3(x, y, z);
  vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
  mat4 projMat = cameraManager->get_projMat();
  vec4 ray_eye = inverse(projMat) * ray_clip;
  ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
  mat4 viewMat = cameraManager->get_viewMat();
  vec4 ray_wor = inverse(viewMat) * ray_eye;
  vec3 ray_world = vec3(ray_wor);
  vec3 ray_dir = normalize(ray_world);

  vec3 cam_pos = cameraManager->get_camPos();
  vec3 P0 = vec3(0,0,0);
  vec3 P1 = vec3(1,0,0);
  vec3 P2 = vec3(0,1,0);

  //Obtain world position at z=0
  vec3 D = ray_dir;
  vec3 N = cross(P1-P0, P2-P0);
  vec3 X = cam_pos + D * dot(P0 - cam_pos, N) / dot(D, N);

  //---------------------------
  return X;
}
