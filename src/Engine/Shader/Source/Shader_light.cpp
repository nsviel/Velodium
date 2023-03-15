#include "Shader_light.h"

#include "../../Node_engine.h"
#include "../../Core/Configuration.h"


Shader_light::Shader_light(){
  //---------------------------

  this->configManager = new Configuration();

  this->name = "light";
  this->light_color = vec3(0, 0, 1);
  this->light_position = vec3(0.0f);

  //---------------------------
}
Shader_light::~Shader_light(){}

void Shader_light::setup_shader(Shader_obj* shader_1, Shader_obj* shader_2){
  this->shader_light = shader_1;
  this->shader_lamp = shader_2;
  //---------------------------

  // Setup shader parameters
  glUseProgram(shader_light->get_program_ID());
  shader_light->setVec3("light_color", light_color);
  shader_light->setVec3("light_position", light_color);

  // Setup shader parameters
  glUseProgram(shader_lamp->get_program_ID());
  shader_lamp->setVec3("light_color", light_color);
  shader_lamp->setVec3("light_position", light_color);

  //---------------------------
  glUseProgram(0);
}
void Shader_light::update_shader(){
  //---------------------------

  // Setup shader parameters
  glUseProgram(shader_light->get_program_ID());
  shader_light->setVec3("light_color", light_color);
  shader_light->setVec3("light_position", light_color);

  // Setup shader parameters
  glUseProgram(shader_lamp->get_program_ID());
  shader_lamp->setVec3("light_color", light_color);
  shader_lamp->setVec3("light_position", light_color);

  //---------------------------
  glUseProgram(0);
}
