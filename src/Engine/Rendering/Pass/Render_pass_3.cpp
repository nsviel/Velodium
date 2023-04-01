#include "Render_pass_3.h"

#include "../../Node_engine.h"
#include "../../Core/Dimension.h"
#include "../../Shader/Shader.h"
#include "../../GPU/GPU_data.h"
#include "../../GPU/GPU_render.h"


//Constructor / Destructor
Render_pass_3::Render_pass_3(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();
  this->shaderManager = node_engine->get_shaderManager();
  this->fboManager = node_engine->get_gpu_fbo();
  this->gpu_data = new GPU_data();
  this->gpu_render = node_engine->get_gpu_render();

  this->canvas = gpu_render->gen_canvas();

  //---------------------------
}
Render_pass_3::~Render_pass_3(){}

//Main function
void Render_pass_3::compute_pass(){
  //---------------------------

  this->bind_canvas();

  //---------------------------
}

//Subfunction
void Render_pass_3::bind_canvas(){
  FBO* gfbo = fboManager->get_fbo_byName("fbo_geometry");
  FBO* fbo_edl = fboManager->get_fbo_byName("fbo_edl");
  FBO* fbo_lvl_0 = fboManager->get_fbo_byName("fbo_py_lvl_2");
  FBO* fbo_visibility = fboManager->get_fbo_byName("fbo_py_visibility");
  FBO* fbo_recombination = fboManager->get_fbo_byName("fbo_recombination");
  FBO* fbo_pass_1 = fboManager->get_fbo_byName("fbo_pass_1");
  //---------------------------
  //Disable depth test
  glDisable(GL_DEPTH_TEST);

    shaderManager->use_shader("shader_canvas");

//LE FBO 1 fait de la MERDEEEE
  //Bind fbo and clear old one
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_pass_1->ID_tex_color);

  vec2 gl_dim = dimManager->get_gl_dim();
  vec2 gl_pos = dimManager->get_gl_pos();
  glViewport(gl_pos.x, gl_pos.y, gl_dim.x, gl_dim.y);

  //Draw quad
  gpu_data->draw_object(canvas);
  gpu_render->unbind_fboAndTexture(1);

  //---------------------------
}
