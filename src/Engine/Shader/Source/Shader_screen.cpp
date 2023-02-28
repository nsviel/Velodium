#include "Shader_screen.h"

#include "../../Node_engine.h"
#include "../../Scene/Configuration.h"


Shader_screen::Shader_screen(Node_engine* node){
  //---------------------------

  this->configManager = node->get_configManager();

  this->name = "screen";
  this->path_vs = "../src/Engine/Shader/glsh/standard/shader_screen.vs";
  this->path_fs = "../src/Engine/Shader/glsh/standard/shader_screen.fs";

  this->with_depth = false;
  this->clip_far = 100;
  this->clip_near = configManager->parse_json_f("camera", "clip_near");
  this->point_size = 10;

  //---------------------------
}
Shader_screen::~Shader_screen(){}

void Shader_screen::setup_shader(Shader_object* shader){
  this->shader = shader;
  //---------------------------

  //Use corresponding shader program
  glUseProgram(shader->get_program_ID());

  // Setup shader parameters
  shader->setBool("depth_ON", with_depth);
  shader->setFloat("near", clip_near);
  shader->setFloat("far", clip_far);
  shader->setInt("point_size", point_size);
  shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

  //---------------------------
}
void Shader_screen::update_shader(){
  //---------------------------

  //Use corresponding shader program
  //glUseProgram(shader->get_program_ID());

  //---------------------------
}
