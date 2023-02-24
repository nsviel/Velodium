#include "Shader_screen.h"

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

  //---------------------------
}
Shader_screen::~Shader_screen(){}

void Shader_screen::setup_shader(GLuint ID){
  this->program_ID = ID;
  //---------------------------

  //Use corresponding shader program
  glUseProgram(program_ID);

//FINIR REGLAGE DU WITH DEPTH VISUALIZATION MODE ET REGLER LES DEPTHS VALUE RANGE E FUNCTION
//Sinon est il possible de limiter le shader edl à certains trucs ?
//IGN

  //Set parameters to shader
  auto with_depth_loc = glGetUniformLocation(program_ID, "depth_ON");
  glUniform1i(with_depth_loc, (int)with_depth);

  auto near_loc = glGetUniformLocation(program_ID, "near");
  auto far_loc = glGetUniformLocation(program_ID, "far");
  glUniform1f(near_loc, (float) clip_near);
  glUniform1f(far_loc, (float) clip_far);

  //---------------------------
}
void Shader_screen::update_shader(){
  //---------------------------

  //Use corresponding shader program
  glUseProgram(program_ID);

  //Set parameters to shader
  auto with_depth_loc = glGetUniformLocation(program_ID, "depth_ON");
  glUniform1i(with_depth_loc, (int)with_depth);

  //---------------------------
}
