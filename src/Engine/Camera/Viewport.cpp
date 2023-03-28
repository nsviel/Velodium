#include "Viewport.h"

#include "../Node_engine.h"
#include "../Core/Dimension.h"
#include "../Core/Configuration.h"



//Constructor / Destructor
Viewport::Viewport(Node_engine* node_engine){

  //---------------------------

  this->dimManager = node_engine->get_dimManager();
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
  view_main.view = "oblique";
  view_main.projection = "perspective";
  view_main.mode = "default";
  view_main.clip_near = configManager->parse_json_f("camera", "clip_near");
  view_main.clip_far = configManager->parse_json_f("camera", "clip_far");

  //---------------------------
  this->nb_viewport++;
}
void Viewport::viewport_init_map(){
  //---------------------------

  view_map.pos = vec2(800,400);
  view_map.dim = vec2(100,100);
  view_map.fov = configManager->parse_json_f("camera", "fov");
  view_map.projection = "perspective";
  view_map.view = "oblique";
  view_map.clip_near = configManager->parse_json_f("camera", "clip_near");
  view_map.clip_far = configManager->parse_json_f("camera", "clip_far");

  //---------------------------
  this->nb_viewport++;
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
