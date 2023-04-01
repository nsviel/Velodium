#include "Render_pass_1.h"

#include "../../Node_engine.h"
#include "../../Core/Dimension.h"
#include "../../Core/Configuration.h"
#include "../../Core/Engine.h"
#include "../../Camera/Camera.h"
#include "../../Shader/Base/Shader_obj.h"
#include "../../Shader/Shader.h"

#include <filesystem>
#include <FreeImage.h>
#include <cstdint>


//Constructor / Destructor
Render_pass_1::Render_pass_1(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();
  this->shaderManager = node_engine->get_shaderManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->engineManager = node_engine->get_engineManager();
  this->configManager = node_engine->get_configManager();
  this->fboManager = node_engine->get_gpu_fbo();

  float bkg_color = configManager->parse_json_f("window", "background_color");
  this->screen_color = vec4(bkg_color, bkg_color, bkg_color, 1.0f);

  //---------------------------
}
Render_pass_1::~Render_pass_1(){}

//Main function
void Render_pass_1::compute_pass(){
  //---------------------------

  this->configure_opengl();
  this->render_simple_mesh();
  this->render_gfbo();

  //---------------------------
}

//Subfunction
void Render_pass_1::configure_opengl(){
  //---------------------------

  //Enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //Set appropriate viewport
  vec2 gl_dim = dimManager->get_gl_dim();
  glViewport(0, 0, gl_dim.x, gl_dim.y);

  //---------------------------
}
void Render_pass_1::render_simple_mesh(){
  //---------------------------

  //Get camera matrices
  mat4 mvp = cameraManager->compute_cam_mvp();

  //Bind first pass fbo
  FBO* fbo_pass_1 = fboManager->get_fbo_byName("fbo_pass_1");
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_pass_1->ID_fbo);

  //Clear framebuffer and enable depth
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Light
  /*Shader_obj* shader_lamp = shaderManager->get_shader_obj_byName("shader_lamp");
  shader_lamp->use();
  shader_lamp->setMat4("MVP", mvp);
  engineManager->draw_light();*/

  //Untextured glyphs
  Shader_obj* shader_untextured = shaderManager->get_shader_obj_byName("shader_mesh_untextured");
  shader_untextured->use();
  shader_untextured->setMat4("MVP", mvp);
  engineManager->draw_untextured_glyph();

  //Textured cloud drawing
  Shader_obj* shader_textured = shaderManager->get_shader_obj_byName("shader_mesh_textured");
  shader_textured->use();
  shader_textured->setMat4("MVP", mvp);
  engineManager->draw_textured_cloud();

  //---------------------------
}
void Render_pass_1::render_gfbo(){
  //-------------------------------

  mat4 view = cameraManager->compute_cam_view();
  mat4 proj = cameraManager->compute_cam_proj();

  //Bind gfbo
  FBO* gfbo = fboManager->get_fbo_byName("fbo_geometry");
  glBindFramebuffer(GL_FRAMEBUFFER, gfbo->ID_fbo);

  //Clear framebuffer and enable depth
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Untextured cloud
  Shader_obj* shader_geometry = shaderManager->get_shader_obj_byName("shader_geometry");
  vec2 gl_pos = dimManager->get_gl_pos();
  shader_geometry->use();
  shader_geometry->setMat4("VIEW", view);
  shader_geometry->setMat4("PROJ", proj);

//say(dimManager->get_gl_pos());
/*
vec3 pt = fct_unproject(vec2(100,80));

glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(pt.x, pt.y, pt.z);
glEnd();*/




  shader_geometry->setVec2("GL_POS", gl_pos);
  engineManager->draw_untextured_cloud();

  //-------------------------------
}
