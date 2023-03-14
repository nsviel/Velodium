#include "Shader_lighting.h"

#include "../../Node_engine.h"
#include "../../Core/Configuration.h"


Shader_lighting::Shader_lighting(){
  //---------------------------

  this->configManager = new Configuration();

  this->name = "lighting";
  this->path_vs = "../src/Engine/Shader/glsh/experimental/shader_lighting.vs";
  this->path_fs = "../src/Engine/Shader/glsh/experimental/shader_lighting.fs";

  this->light_color = vec3(0, 0, 1);

  //---------------------------
}
Shader_lighting::~Shader_lighting(){}

void Shader_lighting::setup_shader(Shader_obj* shader_1, Shader_obj* shader_2){
  this->shader_lighting = shader_1;
  this->shader_lamp = shader_2;
  //---------------------------

  // Setup shader parameters
  glUseProgram(shader_lighting->get_program_ID());
  shader_lighting->setVec3("light_color", light_color);

  // Setup shader parameters
  glUseProgram(shader_lamp->get_program_ID());
  shader_lamp->setVec3("light_color", light_color);

  //---------------------------
  glUseProgram(0);
}
void Shader_lighting::update_shader(){
  //---------------------------

  // Setup shader parameters
  glUseProgram(shader_lighting->get_program_ID());
  shader_lighting->setVec3("light_color", light_color);

  // Setup shader parameters
  glUseProgram(shader_lamp->get_program_ID());
  shader_lamp->setVec3("light_color", light_color);

  //---------------------------
  glUseProgram(0);
}
