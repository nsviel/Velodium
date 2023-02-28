#include "Shader_source.h"


//Constructor / Destructor
Shader_source::Shader_source(){
	//---------------------------

	//---------------------------
}
Shader_source::~Shader_source(){}

void Shader_source::use(){
  //---------------------------

  glUseProgram(shader->get_program_ID());

  //---------------------------
}
