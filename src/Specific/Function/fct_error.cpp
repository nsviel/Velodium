#include "fct_error.h"


void warning_shaderNotFound(std::string name){
  //---------------------------

  std::cout<<"[error] Shader "<<name<<" does not exists."<<std::endl;
	std::cout<<"-> You have to modify the code and make the shader compilable"<<std::endl;

  //---------------------------
}
void error_fileDoesNotExists(std::string path){
  //---------------------------

  std::cout<<"[error] File "<<path<<" does not exists."<<std::endl;

  //---------------------------
}
