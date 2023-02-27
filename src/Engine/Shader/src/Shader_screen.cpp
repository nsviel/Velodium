#include "Shader_screen.h"
#include "Shader_object.h"

#include "../../Node_engine.h"
#include "../../Scene/Configuration.h"


Shader_screen::Shader_screen(Node_engine* node){
  //---------------------------

  this->configManager = node->get_configManager();

  this->path_vs = "../src/Engine/Shader/glsh/shader_screen.vs";
  this->path_fs = "../src/Engine/Shader/glsh/shader_screen.fs";

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

//FINIR REGLAGE DU WITH DEPTH VISUALIZATION MODE ET REGLER LES DEPTHS VALUE RANGE E FUNCTION
//Sinon est il possible de limiter le shader edl à certains trucs ?
//IGN

  //Set parameters to shader
  auto with_depth_loc = glGetUniformLocation(shader->get_program_ID(), "depth_ON");
  glUniform1i(with_depth_loc, (int)with_depth);

  auto near_loc = glGetUniformLocation(shader->get_program_ID(), "near");
  auto far_loc = glGetUniformLocation(shader->get_program_ID(), "far");
  glUniform1f(near_loc, clip_near);
  glUniform1f(far_loc, clip_far);

  auto point_size_loc = glGetUniformLocation(shader->get_program_ID(), "point_size");
  glUniform1i(point_size_loc, point_size);

  auto lightColor_loc = glGetUniformLocation(shader->get_program_ID(), "lightColor");
  //glUniform1i(lightColor_loc, vec3(1.0f, 1.0f, 1.0f));

  /*
  //Use corresponding shader program
  glUseProgram(shader->get_program_ID());

  // Setup shader parameters
  shader->setBool("depth_ON", with_depth);
  shader->setFloat("near", clip_near);
  shader->setFloat("far", clip_far);
  shader->setInt("point_size", point_size);
  shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
  */

  //---------------------------
}
void Shader_screen::update_shader(){
  //---------------------------

  //Use corresponding shader program
  //glUseProgram(shader->get_program_ID());

  //---------------------------
}
