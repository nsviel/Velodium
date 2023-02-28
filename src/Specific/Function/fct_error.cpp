#include "fct_error.h"


void warning_shaderNotFound(std::string name){
  //---------------------------

  std::cout<<"[error] Shader "<<name<<" does not exists, sorry."<<std::endl;
	std::cout<<"-> You have to modify the code and make the shader compilable"<<std::endl;

  //---------------------------
}
