#include "GPU_pyramid.h"

#include "../GPU/GPU_data.h"
#include "../GPU/GPU_fbo.h"
#include "../Shader/Base/Shader_obj.h"
#include "../Node_engine.h"
#include "../Shader/Shader.h"
#include "../Camera/Camera.h"
#include "../Core/Dimension.h"


//Constructor / Destructor
GPU_pyramid::GPU_pyramid(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();
  this->shaderManager = node_engine->get_shaderManager();
  this->gpuManager = new GPU_data();
  this->fboManager = node_engine->get_fboManager();
  this->cameraManager = node_engine->get_cameraManager();

  //---------------------------
}
GPU_pyramid::~GPU_pyramid(){}

//Main function
void GPU_pyramid::bind_pyramid(Object_* canvas){
  //---------------------------

  //Activate depth buffering
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_ALWAYS);

  this->bind_pyramid_lvl_0(canvas);
  this->bind_pyramid_lvl_n(canvas);
  this->bind_pyramid_visibility(canvas);

  glDisable(GL_DEPTH_TEST);

  //---------------------------
}
void GPU_pyramid::bind_pyramid_lvl_0(Object_* canvas){
  FBO* gfbo = fboManager->get_fbo_byName("fbo_geometry");
  FBO* fbo_lvl_0 = fboManager->get_fbo_byName("fbo_py_lvl_0");
  //---------------------------

  //Pyramide level 0
  Shader_obj* shader_lvl_0 = shaderManager->get_shader_obj_byName("shader_py_lvl_0");
  shader_lvl_0->use();

  vec2 gl_dim = dimManager->get_win_dim();
  shader_lvl_0->setInt("GL_WIDTH", gl_dim.x);
  shader_lvl_0->setInt("GL_HEIGHT", gl_dim.y);

  mat4 view = cameraManager->get_cam_view();
  shader_lvl_0->setMat4("VIEW", view);

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
  Shader_obj* shader_lvl_n = shaderManager->get_shader_obj_byName("shader_py_lvl_n");
  shader_lvl_n->use();

  //Next pyramid level
  for(int i=1; i<struct_pyramid->nb_lvl; i++){
    FBO* fbo_lvl_m1 = struct_pyramid->fbo_vec[i-1];
    FBO* fbo_lvl_m0 = struct_pyramid->fbo_vec[i];

    shader_lvl_n->setInt("NN_SIZE", struct_pyramid->size_nn[i]);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_lvl_m0->ID_fbo);

    //Input: read textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_lvl_m1->ID_buffer_depth);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbo_lvl_m1->ID_tex_position);

    gpuManager->draw_object(canvas);
    this->unbind_fboAndTexture(2);
  }

  //---------------------------
}
void GPU_pyramid::bind_pyramid_visibility(Object_* canvas){
  Pyramid* struct_pyramid = fboManager->get_struct_pyramid();
  //---------------------------

  //Get fbo pointer
  FBO* gfbo = fboManager->get_fbo_byName("fbo_geometry");
  FBO* fbo_visibility = fboManager->get_fbo_byName("fbo_py_visibility");
  FBO* fbo_lvl_0 = struct_pyramid->fbo_vec[0];
  FBO* fbo_lvl_1 = struct_pyramid->fbo_vec[1];
  FBO* fbo_lvl_2 = struct_pyramid->fbo_vec[2];
  FBO* fbo_lvl_3 = struct_pyramid->fbo_vec[2];
  FBO* fbo_lvl_4 = struct_pyramid->fbo_vec[2];

  //Use shader
  Shader_obj* shader_visibility = shaderManager->get_shader_obj_byName("shader_py_visibility");
  shader_visibility->use();

  //Set FBO
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_visibility->ID_fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  vec3 cam_pose = cameraManager->get_cam_P();
  shader_visibility->setVec3("CAM_POSE", cam_pose);

  mat4 proj = cameraManager->get_cam_proj();
  shader_visibility->setMat4("PROJ", proj);

  mat4 view = cameraManager->get_cam_view();
  shader_visibility->setMat4("VIEW", view);

  GLint samplers[5] = { 0, 1, 2, 3, 4 };
  shader_visibility->setIntVec("tex_position", samplers);




  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_buffer_depth);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_tex_color);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_0->ID_tex_position);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_1->ID_tex_position);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_2->ID_tex_position);
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_3->ID_tex_position);
  glActiveTexture(GL_TEXTURE6);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_4->ID_tex_position);

  gpuManager->draw_object(canvas);
  this->unbind_fboAndTexture(struct_pyramid->nb_lvl+1);

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
