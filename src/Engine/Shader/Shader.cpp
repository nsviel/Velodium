#include "Shader.h"

#include "PP_edl.h"
#include "ShaderObject.h"

#include "../OpenGL/Dimension.h"



//Constructor / Destructor
Shader::Shader(Dimension* dimManager){
	//---------------------------

	this->edlManager = new PP_edl(dimManager);

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
void Shader::update(){
	//---------------------------

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
