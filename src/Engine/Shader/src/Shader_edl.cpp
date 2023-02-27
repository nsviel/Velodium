#include "Shader_edl.h"
#include "Shader_object.h"

#include "../../Node_engine.h"
#include "../../Core/Dimension.h"
#include "../../Scene/Configuration.h"


Shader_edl::Shader_edl(Node_engine* node){
  //---------------------------

  this->dimManager = node->get_dimManager();
  this->configManager = node->get_configManager();

  this->path_vs = "../src/Engine/Shader/glsh/shader_edl.vs";
  this->path_fs = "../src/Engine/Shader/glsh/shader_edl.fs";

  this->with_edl = true;
  this->with_inv = false;
  this->edl_strength = 100.0;
  this->edl_distance = 1.0;
  this->edl_radius = 1.0;
  this->clip_far = configManager->parse_json_f("camera", "clip_far");
  this->clip_near = configManager->parse_json_f("camera", "clip_near");
  //this->clip_far = configManager->parse_json_f("camera", "clip_far");
  //this->clip_near = configManager->parse_json_f("camera", "clip_near");

  //---------------------------
}
Shader_edl::~Shader_edl(){}

void Shader_edl::setup_shader(Shader_object* shader){
  this->shader = shader;
  int program_ID =shader->get_program_ID();
  //---------------------------

  //Use corresponding shader program
  glUseProgram(program_ID);

  // Setup shader parameters
  auto a = (clip_far + clip_near) / (clip_far - clip_near);
  auto b = (-2 * clip_far * clip_near) / (clip_far - clip_near);
  shader->setFloat("A", a);
  shader->setFloat("B", b);

  shader->setFloat("EDL_STRENGTH", edl_strength);
  shader->setFloat("EDL_DISTANCE", edl_distance);
  shader->setFloat("EDL_RADIUS", edl_radius);
  shader->setInt("EDL_ON", with_edl);

  shader->setInt("tex_color", 0);
  shader->setInt("tex_depth", 1);

  vec2 gl_dim = dimManager->get_gl_dim();
  shader->setInt("GL_WIDTH", gl_dim.x);
  shader->setInt("GL_HEIGHT", gl_dim.y);

  //---------------------------
}
void Shader_edl::update_shader(){
  //---------------------------
int program_ID =shader->get_program_ID();
  //Use corresponding shader program
  glUseProgram(program_ID);

  // Setup shader parameters
  auto a = (clip_far + clip_near) / (clip_far - clip_near);
  auto b = (-2 * clip_far * clip_near) / (clip_far - clip_near);
  shader->setFloat("A", a);
  shader->setFloat("B", b);

  shader->setFloat("EDL_STRENGTH", edl_strength);
  shader->setFloat("EDL_DISTANCE", edl_distance);
  shader->setFloat("EDL_RADIUS", edl_radius);
  shader->setInt("EDL_ON", with_edl);

  vec2 gl_dim = dimManager->get_gl_dim();
  shader->setInt("GL_WIDTH", gl_dim.x);
  shader->setInt("GL_HEIGHT", gl_dim.y);

  //---------------------------
}
