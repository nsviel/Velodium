#include "Shader.h"

#include "PP_edl.h"
#include "ShaderObject.h"

#include "../Dimension.h"



//Constructor / Destructor
Shader::Shader(Dimension* dimManager){
	//---------------------------

	this->edlManager = new PP_edl(dimManager);

	//---------------------------
}
Shader::~Shader(){}

void Shader::init(){
	//---------------------------

	string path_scene_vs = "../src/Engine/OpenGL/Shader/shader_scene.vs";
	string path_scene_fs = "../src/Engine/OpenGL/Shader/shader_scene.fs";

	string path_screen_vs = "../src/Engine/OpenGL/Shader/shader_edl.vs";
	string path_screen_fs = "../src/Engine/OpenGL/Shader/shader_edl.fs";

	shader_scene = new ShaderObject(path_scene_vs, path_scene_fs);
	shader_screen = new ShaderObject(path_screen_vs, path_screen_fs);

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
