#include "Shader_pyramid.h"

#include "../../Node_engine.h"
#include "../../Core/Dimension.h"
#include "../../Core/Configuration.h"


Shader_pyramid::Shader_pyramid(Node_engine* node){
  //---------------------------

  this->dimManager = node->get_dimManager();
  this->configManager = node->get_configManager();

  this->name = "pyramid";
  this->path_vs = "../src/Engine/Shader/glsh/pass_2/shader_pyramid.vs";
  this->path_fs = "../src/Engine/Shader/glsh/pass_2/shader_pyramid.fs";

  this->clip_far = configManager->parse_json_f("camera", "clip_far");
  this->clip_near = configManager->parse_json_f("camera", "clip_near");

  //---------------------------
}
Shader_pyramid::~Shader_pyramid(){}

void Shader_pyramid::setup_shader(Shader_obj* shader){
  this->shader = shader;
  //---------------------------

  //Use corresponding shader program
  this->use();

  // Setup shader parameters
  shader->setFloat("Z_NEAR", clip_near);
  shader->setFloat("Z_FAR", clip_far);

  //A AUTOMATISER ICI !!!!
  shader->setInt("tex_color", 0);
  shader->setInt("tex_depth", 1);

  vec2 gl_dim = dimManager->get_gl_dim();
  shader->setInt("GL_WIDTH", gl_dim.x);
  shader->setInt("GL_HEIGHT", gl_dim.y);
  shader->setInt("NN_SIZE", 2);

  //---------------------------
}
void Shader_pyramid::update_shader(){
  //---------------------------

  //Use corresponding shader program
  this->use();

  // Setup shader parameters
  shader->setFloat("Z_NEAR", clip_near);
  shader->setFloat("Z_FAR", clip_far);

  vec2 gl_dim = dimManager->get_gl_dim();
  shader->setInt("GL_WIDTH", gl_dim.x);
  shader->setInt("GL_HEIGHT", gl_dim.y);
  shader->setInt("NN_SIZE", 2);

  //---------------------------
}
