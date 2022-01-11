#include "PP_edl.h"

#include "Shader.h"


PP_edl::PP_edl(){}
PP_edl::~PP_edl(){}

void PP_edl::setup_edl(Shader* shader){
  auto program_ID = shader->get_program_ID();
  glUseProgram(program_ID);
  //---------------------------

  //parameters
  bool with_edl = true;
  double edl_strength = 100.0;
  double edl_distance = 1.0;
  double clip_far = 10000.0;
  double clip_near = 0.1;

  //Set parameters to shader
  auto a_loc = glGetUniformLocation(program_ID, "A");
  auto b_loc = glGetUniformLocation(program_ID, "B");
  auto a = -(clip_far + clip_near) / (clip_far - clip_near);
  auto b = (-2 * clip_far * clip_near) / (clip_far - clip_near);
  glUniform1f(a_loc, (float) a);
  glUniform1f(b_loc, (float) b);

  auto edl_stgh_loc = glGetUniformLocation(program_ID, "EDL_STRENGTH");
  auto edl_dist_loc = glGetUniformLocation(program_ID, "EDL_DISTANCE");
  auto with_edl_loc = glGetUniformLocation(program_ID, "EDL_ON");

  glUniform1f(edl_stgh_loc, (float)edl_strength);
  glUniform1f(edl_dist_loc, (float)edl_distance);
  glUniform1i(with_edl_loc, (int)with_edl);

  auto color_texture_loc = glGetUniformLocation(program_ID, "tex_color");
  auto depth_texture_loc = glGetUniformLocation(program_ID, "tex_depth");
  glUniform1i(color_texture_loc, 0);
  glUniform1i(depth_texture_loc, 1);

  //---------------------------
}
void PP_edl::setup_textures(GLuint tex_color, GLuint tex_depth){
  this->tex_color_ID = tex_color;
  this->tex_depth_ID = tex_depth;
}
