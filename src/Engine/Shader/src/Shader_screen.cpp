#include "Shader_screen.h"


Shader_screen::Shader_screen(){
  //---------------------------

  this->path_vs = "../src/Engine/Shader/glsh/shader_screen.vs";
  this->path_fs = "../src/Engine/Shader/glsh/shader_screen.fs";

  this->with_depth = false;

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

  //Set parameters to shader
  auto with_depth_loc = glGetUniformLocation(program_ID, "depth_ON");
  glUniform1i(with_depth_loc, (int)with_depth);

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
