#include "Shader_screen.h"


Shader_screen::Shader_screen(){
  //---------------------------

  this->with_depth = true;

  //---------------------------
}
Shader_screen::~Shader_screen(){}

void Shader_screen::setup_shader(GLuint program_ID){
  glUseProgram(program_ID);
  //---------------------------
//FINIR REGLAGE DU WITH DEPTH VISUALIZATION MODE ET REGLER LES DEPTHS VALUE RANGE E FUNCTION
  //Set parameters to shader
  auto with_depth_loc = glGetUniformLocation(program_ID, "with_depth");
  glUniform1i(with_depth_loc, (int)with_depth);

  //---------------------------
}
