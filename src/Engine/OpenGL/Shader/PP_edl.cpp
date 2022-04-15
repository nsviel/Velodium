#include "PP_edl.h"

#include "../Dimension.h"



PP_edl::PP_edl(Dimension* dim){
  //---------------------------

  this->dimManager = dim;

  this->with_edl = true;
  this->edl_strength = 500.0;
  this->edl_distance = 1.0;
  this->edl_radius = 1.0;
  this->clip_far = 10000.0;
  this->clip_near = 0.1;

  //---------------------------
}
PP_edl::~PP_edl(){}

void PP_edl::setup_edl(GLuint program_ID){
  glUseProgram(program_ID);
  //---------------------------

  //Set parameters to shader
  auto a_loc = glGetUniformLocation(program_ID, "A");
  auto b_loc = glGetUniformLocation(program_ID, "B");
  auto a = -(clip_far + clip_near) / (clip_far - clip_near);
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
void PP_edl::setup_textures(GLuint tex_color, GLuint tex_depth){
  this->tex_color_ID = tex_color;
  this->tex_depth_ID = tex_depth;
}
