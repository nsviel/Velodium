#include "Shader_screen.h"

#include "../../Node_engine.h"
#include "../../Core/Configuration.h"


Shader_screen::Shader_screen(Node_engine* node){
  //---------------------------

  this->configManager = node->get_configManager();

  this->name = "screen";
  this->path_vs = "../src/Engine/Shader/glsh/pass_1/mesh_untextured.vs";
  this->path_fs = "../src/Engine/Shader/glsh/pass_1/mesh_untextured.fs";

  this->clip_far = configManager->parse_json_f("camera", "clip_far");
  this->clip_near = configManager->parse_json_f("camera", "clip_near");
  this->point_size = 10;

  //---------------------------
}
Shader_screen::~Shader_screen(){}

void Shader_screen::setup_shader(Shader_object* shader){
  this->shader = shader;
  //---------------------------

  //Use corresponding shader program
  this->use();

  // Setup shader parameters
  shader->setFloat("near", clip_near);
  shader->setFloat("far", clip_far);
  shader->setInt("point_size", point_size);
  shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

  //---------------------------
}
void Shader_screen::update_shader(){
  //---------------------------

  //Use corresponding shader program
  //this->use();

  //---------------------------
}
