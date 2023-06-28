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
  this->gpu_fbo = node_engine->get_gpu_fbo();
  this->gpu_render = node_engine->get_gpu_render();
  this->gpu_data = new GPU_data();

  this->canvas = gpu_render->gen_canvas();

  //---------------------------
}
Render_pass_3::~Render_pass_3(){}

//Main function
void Render_pass_3::compute_pass(){
  //---------------------------

  this->configure_opengl();
  this->render_canvas_fbo();
  this->render_canvas_draw();

  //---------------------------
}

//Subfunction
void Render_pass_3::configure_opengl(){
  //---------------------------

  //Disable depth test
  glDisable(GL_DEPTH_TEST);

  //Set appropriate viewport
  vec2 gl_dim = dimManager->get_gl_dim();
  vec2 gl_pos = dimManager->get_gl_pos();
  glViewport(gl_pos.x, gl_pos.y, gl_dim.x, gl_dim.y);

  //---------------------------
}
void Render_pass_3::render_canvas_fbo(){
  //---------------------------

  //Bind shader
  shaderManager->use_shader("shader_canvas");

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void Render_pass_3::render_canvas_draw(){
  FBO* fbo_recombination = gpu_fbo->get_fbo_byName("fbo_pass_1");
  FBO* fbo_edl = gpu_fbo->get_fbo_byName("fbo_edl");
  //---------------------------

  //Bind texture-to-render
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_edl->ID_tex_color);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Draw quad
  gpu_data->draw_object(canvas);
  gpu_render->unbind_fboAndTexture(1);

  //---------------------------
}
