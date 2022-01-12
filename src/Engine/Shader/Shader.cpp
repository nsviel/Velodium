#include "Shader.h"

#include "PP_edl.h"

#include <fstream>

#include "ShaderObject.h"


//Constructor / Destructor
Shader::Shader(){
	//---------------------------

	this->edlManager = new PP_edl();

	//---------------------------
}
Shader::~Shader(){}

void Shader::init(){
	//---------------------------

	shader_scene = new ShaderObject("../src/Engine/Shader/shader_scene.vs", "../src/Engine/Shader/shader_scene.fs");
	shader_screen = new ShaderObject("../src/Engine/Shader/shader_edl.vs", "../src/Engine/Shader/shader_edl.fs");

	edlManager->setup_edl(shader_screen->get_program_ID());

	//---------------------------
}

void Shader::use(string shader_name){
	//---------------------------

	if(shader_name == "scene"){
		shader_scene->use();
	}
	else if(shader_name == "screen"){
		shader_screen->use();
	}

	//---------------------------
}
void Shader::use_screen(){
	shader_screen->use();
}
void Shader::use_scene(){
	//---------------------------

	shader_scene->use();

	//---------------------------
}
