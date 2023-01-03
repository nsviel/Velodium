#include "Viewport.h"

#include "../Core/Dimension.h"
#include "../Scene/Configuration.h"



//Constructor / Destructor
Viewport::Viewport(Dimension* dimension){
  this->dimManager = dimension;
  //---------------------------

  this->configManager = new Configuration();

  this->nb_viewport = 2;

  //---------------------------
  this->viewport_init();
}
Viewport::~Viewport(){}

void Viewport::viewport_init(){
  //---------------------------

  this->viewport_init_main();
  this->viewport_init_map();

  //---------------------------
}
void Viewport::viewport_init_main(){
  //---------------------------

  float camPos = configManager->parse_json_f("camera", "initial_pos");
  view_main.cam_P = vec3(camPos, camPos, camPos);
  view_main.speed_move = configManager->parse_json_f("camera", "speed_move");
  view_main.speed_mouse = configManager->parse_json_f("camera", "speed_mouse");
  view_main.angle_azimuth = M_PI + M_PI/4;// Initial horizontal angle
  view_main.angle_elevation = - M_PI/6;// Initial vertical angle
  view_main.cam_R = normalize(vec3(cos(view_main.angle_azimuth - M_PI/2.0f), sin(view_main.angle_azimuth - M_PI/2.0f), 0));
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

  //---------------------------
  this->nb_viewport++;
}
void Viewport::viewport_init_map(){
  //---------------------------

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
  this->nb_viewport++;
}
void Viewport::viewport_update(int ID){
  //---------------------------

  //Main viewport
  if(ID == 0){
    view_main.dim = dimManager->get_gl_dim();
    view_main.pos = dimManager->get_gl_pos();
    glViewport(0, 0, view_main.dim.x, view_main.dim.y);
  }
  //Map viewport
  else if(ID == 1){
    glViewport(view_map.pos.x, view_map.pos.y, view_map.dim.x, view_map.dim.y);
  }

  //---------------------------
}
void Viewport::viewport_reset(){
  //---------------------------

  float camPos = configManager->parse_json_f("camera", "initial_pos");
  view_main.cam_P = vec3(camPos, camPos, camPos);
  view_main.angle_azimuth = M_PI + M_PI/4;// Initial horizontal angle
  view_main.angle_elevation = - M_PI/6;// Initial vertical angle
  view_main.cam_R = normalize(vec3(cos(view_main.angle_azimuth - M_PI/2.0f), sin(view_main.angle_azimuth - M_PI/2.0f), 0));

  //---------------------------
}

void Viewport::update_viewport(){
  //---------------------------

  dimManager->update_window_dim();
  vec2 dim = dimManager->get_gl_dim();
  vec2 pos = dimManager->get_gl_pos();

  glViewport(pos.x, pos.y, dim.x/2, dim.y/2);

  //---------------------------
}
void Viewport::update_viewport(int loop_cpt){
  //---------------------------

  dimManager->update_window_dim();
  vec2 dim = dimManager->get_gl_dim();
  vec2 pos = dimManager->get_gl_pos();

  if(loop_cpt == 0){
    glViewport(pos.x, pos.y, dim.x, dim.y);

  }else{
    glViewport(pos.x + dim.x/2, pos.y + dim.y/2, dim.x/2, dim.y/2);
  }

  //cameraManager->input_cam_mouse();

  //---------------------------
}
