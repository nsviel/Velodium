#include "Render_pyramid.h"

#include "../../GPU/GPU_data.h"
#include "../../GPU/GPU_fbo.h"
#include "../../GPU/GPU_render.h"
#include "../../Shader/Base/Shader_obj.h"
#include "../../Node_engine.h"
#include "../../Shader/Shader.h"
#include "../../Camera/Camera.h"
#include "../../Core/Dimension.h"


//Constructor / Destructor
Render_pyramid::Render_pyramid(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();
  this->shaderManager = node_engine->get_shaderManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->gpu_fbo = node_engine->get_gpu_fbo();
  this->gpu_render = node_engine->get_gpu_render();
  this->gpu_data = new GPU_data();

  //---------------------------
}
Render_pyramid::~Render_pyramid(){}

//Main function
void Render_pyramid::render_pyramid(Object_* canvas){
  //---------------------------

  this->pyramid_lvl_0(canvas);
  this->pyramid_lvl_n(canvas);
  this->pyramid_visibility(canvas);

  //---------------------------
}

//Subfunction
void Render_pyramid::pyramid_lvl_0(Object_* canvas){
  Shader_obj* shader_lvl_0 = shaderManager->get_shader_obj_byName("shader_py_lvl_0");
  FBO* gfbo = gpu_fbo->get_fbo_byName("fbo_geometry");
  FBO* fbo_lvl_0 = gpu_fbo->get_fbo_byName("fbo_py_lvl_0");
  vec2 gl_dim = dimManager->get_gl_dim();
  mat4 view = cameraManager->get_cam_view();
  //---------------------------

  //Pyramide level 0
  shader_lvl_0->use();

  //Set parameters
  shader_lvl_0->setInt("GL_WIDTH", gl_dim.x);
  shader_lvl_0->setInt("GL_HEIGHT", gl_dim.y);
  shader_lvl_0->setMat4("VIEW", view);

  //Bind pyramid level
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_lvl_0->ID_fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_buffer_depth);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_tex_position);

  //Draw render canvas
  gpu_data->draw_object(canvas);
  gpu_render->unbind_fboAndTexture(2);

  //---------------------------
}
void Render_pyramid::pyramid_lvl_n(Object_* canvas){
  Shader_obj* shader_lvl_n = shaderManager->get_shader_obj_byName("shader_py_lvl_n");
  Pyramid* struct_pyramid = gpu_fbo->get_struct_pyramid();
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Pyramid level n
  shader_lvl_n->use();

  //Next pyramid level
  for(int i=1; i<struct_pyramid->nb_lvl; i++){

  }
  FBO* fbo_lvl_0 = gpu_fbo->get_fbo_byName("fbo_py_lvl_0");
  FBO* fbo_lvl_1 = gpu_fbo->get_fbo_byName("fbo_py_lvl_1");

  FBO* fbo_lvl_m1 = struct_pyramid->fbo_vec[0];
  FBO* fbo_lvl_m0 = struct_pyramid->fbo_vec[1];

  //Set parameters
  shader_lvl_n->setInt("GL_WIDTH", gl_dim.x);
  shader_lvl_n->setInt("GL_HEIGHT", gl_dim.y);
  shader_lvl_n->setInt("NN_SIZE", struct_pyramid->size_nn[1]);
  shader_lvl_n->setInt("tex_depth", 0);
  shader_lvl_n->setInt("tex_position", 1);

  //Bind pyramid level
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_lvl_1->ID_fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_0->ID_buffer_depth);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_0->ID_tex_position);

  gpu_data->draw_object(canvas);
  gpu_render->unbind_fboAndTexture(2);

  //---------------------------
}
void Render_pyramid::pyramid_visibility(Object_* canvas){
  Shader_obj* shader_visibility = shaderManager->get_shader_obj_byName("shader_py_visibility");
  FBO* gfbo = gpu_fbo->get_fbo_byName("fbo_geometry");
  FBO* fbo_visibility = gpu_fbo->get_fbo_byName("fbo_py_visibility");
  Pyramid* struct_pyramid = gpu_fbo->get_struct_pyramid();
  //---------------------------

  //Get fbo pointer
  FBO* fbo_lvl_0 = struct_pyramid->fbo_vec[0];
  FBO* fbo_lvl_1 = struct_pyramid->fbo_vec[1];
  FBO* fbo_lvl_2 = struct_pyramid->fbo_vec[2];
  FBO* fbo_lvl_3 = struct_pyramid->fbo_vec[2];
  FBO* fbo_lvl_4 = struct_pyramid->fbo_vec[2];

  //Get parameter values
  vec3 cam_pose = cameraManager->get_cam_P();
  mat4 proj = cameraManager->get_cam_proj();
  mat4 view = cameraManager->get_cam_view();
  vec2 gl_dim = dimManager->get_gl_dim();
  GLint samplers[5] = { 0, 1, 2, 3, 4 };

  //Use shader
  shader_visibility->use();

  //Bind visibility fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_visibility->ID_fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Set parameters
  shader_visibility->setVec3("CAM_POSE", cam_pose);
  shader_visibility->setMat4("PROJ", proj);
  shader_visibility->setMat4("VIEW", view);
  shader_visibility->setIntVec("tex_position", samplers);
  shader_visibility->setInt("GL_WIDTH", gl_dim.x);
  shader_visibility->setInt("GL_HEIGHT", gl_dim.y);
//  say(cam_pose);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_buffer_depth);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_tex_color);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_tex_position);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_0->ID_tex_color);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_1->ID_tex_color);
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_2->ID_tex_color);
  glActiveTexture(GL_TEXTURE6);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_3->ID_tex_color);
  glActiveTexture(GL_TEXTURE7);
  glBindTexture(GL_TEXTURE_2D, fbo_lvl_4->ID_tex_color);

  //Draw render canvas
  gpu_data->draw_object(canvas);
  gpu_render->unbind_fboAndTexture(struct_pyramid->nb_lvl+1);

  //---------------------------
}
