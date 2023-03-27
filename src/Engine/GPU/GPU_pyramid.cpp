#include "GPU_pyramid.h"
#include "GPU_data.h"
#include "GPU_fbo.h"

#include "../Shader/Base/Shader_obj.h"
#include "../Node_engine.h"
#include "../Shader/Shader.h"


//Constructor / Destructor
GPU_pyramid::GPU_pyramid(Node_engine* node_engine){
  //---------------------------

  this->shaderManager = node_engine->get_shaderManager();
  this->gpuManager = new GPU_data();
  this->fboManager = node_engine->get_fboManager();

  //---------------------------
}
GPU_pyramid::~GPU_pyramid(){}

//Main function
void GPU_pyramid::bind_pyramid(Object_* canvas){
  //---------------------------

  this->bind_pyramid_lvl_0(canvas);
  this->bind_pyramid_lvl_n(canvas);
  this->bind_pyramid_visibility(canvas);

  //---------------------------
}
void GPU_pyramid::bind_pyramid_lvl_0(Object_* canvas){
  FBO* gfbo = fboManager->get_fbo_byName("gfbo");
  FBO* fbo_lvl_0 = fboManager->get_fbo_byName("pyramid_0");
  //---------------------------

  //Pyramide level 0
  shaderManager->use_shader("pyramid_lvl_0");

  //First pyramid level
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_lvl_0->ID_fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_buffer_depth);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_tex_position);

  gpuManager->draw_object(canvas);
  this->unbind_fboAndTexture(2);

  //---------------------------
}
void GPU_pyramid::bind_pyramid_lvl_n(Object_* canvas){
  Pyramid* struct_pyramid = fboManager->get_struct_pyramid();
  //---------------------------

  //Pyramid level n
  Shader_obj* shader_lvl_n = shaderManager->get_shader_obj_byName("pyramid_lvl_n");
  shader_lvl_n->use();

  //Next pyramid level
  for(int i=1; i<struct_pyramid->nb_lvl; i++){
    FBO* fbo_lvl_m1 = struct_pyramid->fbo_vec[i-1];
    FBO* fbo_lvl_m0 = struct_pyramid->fbo_vec[i];

    shader_lvl_n->setInt("NN_SIZE", i*2);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_lvl_m0->ID_fbo);

    //Input: read textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_lvl_m1->ID_tex_color);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbo_lvl_m1->ID_tex_position);

    gpuManager->draw_object(canvas);
    this->unbind_fboAndTexture(1);
  }

  //---------------------------
}
void GPU_pyramid::bind_pyramid_visibility(Object_* canvas){
  Pyramid* struct_pyramid = fboManager->get_struct_pyramid();
  //---------------------------

  //Get fbo pointer
  FBO* fbo_visibility = fboManager->get_fbo_byName("pyramid_visibility");
  FBO* fbo_lvl_0 = struct_pyramid->fbo_vec[0];
  FBO* fbo_lvl_1 = struct_pyramid->fbo_vec[1];
  FBO* fbo_lvl_2 = struct_pyramid->fbo_vec[2];

  //Use shader
  shaderManager->use_shader("pyramid_visibility");

  //Set FBO
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_visibility->ID_fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_0->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_1->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_2->ID_tex_color);

  gpuManager->draw_object(canvas);
  this->unbind_fboAndTexture(2);

  //---------------------------
}

//Subfunction
void GPU_pyramid::unbind_fboAndTexture(int nb_tex){
  //---------------------------

  //Unbind texture
  for(int i=0; i<nb_tex; i++){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  //Unbind FBO
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
