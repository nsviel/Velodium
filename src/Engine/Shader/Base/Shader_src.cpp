#include "Shader_src.h"


//Constructor / Destructor
Shader_src::Shader_src(){
	//---------------------------

	//---------------------------
}
Shader_src::~Shader_src(){}

void Shader_src::use(){
  //---------------------------

  glUseProgram(shader->get_program_ID());

  //---------------------------
}
