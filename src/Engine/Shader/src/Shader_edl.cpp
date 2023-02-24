#include "Shader_edl.h"

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

void Shader_edl::setup_shader(GLuint ID){
  this->program_ID = ID;
  //---------------------------

  //Use corresponding shader program
  glUseProgram(program_ID);

  //Set parameters to shader
  auto a_loc = glGetUniformLocation(program_ID, "A");
  auto b_loc = glGetUniformLocation(program_ID, "B");
  auto a = (clip_far + clip_near) / (clip_far - clip_near);
  auto b = (-2 * clip_far * clip_near) / (clip_far - clip_near);
  glUniform1f(a_loc, (float) a);
  glUniform1f(b_loc, (float) b);

  auto edl_stgh_loc = glGetUniformLocation(program_ID, "EDL_STRENGTH");
  auto edl_dist_loc = glGetUniformLocation(program_ID, "EDL_DISTANCE");
  auto edl_radi_loc = glGetUniformLocation(program_ID, "EDL_RADIUS");
  auto with_edl_loc = glGetUniformLocation(program_ID, "EDL_ON");

  glUniform1f(edl_stgh_loc, (float)edl_strength);
  glUniform1f(edl_dist_loc, (float)edl_distance);
  glUniform1f(edl_radi_loc, (float)edl_radius);
  glUniform1i(with_edl_loc, (int)with_edl);

  auto color_texture_loc = glGetUniformLocation(program_ID, "tex_color");
  auto depth_texture_loc = glGetUniformLocation(program_ID, "tex_depth");
  glUniform1i(color_texture_loc, 0);
  glUniform1i(depth_texture_loc, 1);

  vec2 gl_dim = dimManager->get_gl_dim();
  auto egl_width_loc = glGetUniformLocation(program_ID, "GL_WIDTH");
  auto edl_height_loc = glGetUniformLocation(program_ID, "GL_HEIGHT");
  glUniform1i(egl_width_loc, gl_dim.x);
  glUniform1i(edl_height_loc, gl_dim.y);

  //---------------------------
}
void Shader_edl::update_shader(){
  //---------------------------

  //Use corresponding shader program
  glUseProgram(program_ID);

  //Set parameters to shader
  auto a_loc = glGetUniformLocation(program_ID, "A");
  auto b_loc = glGetUniformLocation(program_ID, "B");
  auto a = (clip_far + clip_near) / (clip_far - clip_near);
  auto b = (-2 * clip_far * clip_near) / (clip_far - clip_near);
  glUniform1f(a_loc, (float) a);
  glUniform1f(b_loc, (float) b);

  auto edl_stgh_loc = glGetUniformLocation(program_ID, "EDL_STRENGTH");
  auto edl_dist_loc = glGetUniformLocation(program_ID, "EDL_DISTANCE");
  auto edl_radi_loc = glGetUniformLocation(program_ID, "EDL_RADIUS");
  auto with_edl_loc = glGetUniformLocation(program_ID, "EDL_ON");
  auto with_inv_loc = glGetUniformLocation(program_ID, "invertion_ON");

  glUniform1f(edl_stgh_loc, (float)edl_strength);
  glUniform1f(edl_dist_loc, (float)edl_distance);
  glUniform1f(edl_radi_loc, (float)edl_radius);
  glUniform1i(with_edl_loc, (int)with_edl);
  glUniform1i(with_inv_loc, (int)with_inv);

  vec2 gl_dim = dimManager->get_gl_dim();
  auto egl_width_loc = glGetUniformLocation(program_ID, "GL_WIDTH");
  auto edl_height_loc = glGetUniformLocation(program_ID, "GL_HEIGHT");
  glUniform1i(egl_width_loc, gl_dim.x);
  glUniform1i(edl_height_loc, gl_dim.y);

  //---------------------------
}
