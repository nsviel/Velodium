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

void Shader_lighting::setup_shader(Shader_object* shader){
  this->shader = shader;
  //---------------------------

  //Use corresponding shader program
  this->use();

  // Setup shader parameters
  shader->setVec3("light_color", light_color);

  //---------------------------
}
void Shader_lighting::update_shader(){
  //---------------------------

  //Use corresponding shader program
  this->use();

  //Update shader parameters
  shader->setVec3("light_color", light_color);

  //---------------------------
}
