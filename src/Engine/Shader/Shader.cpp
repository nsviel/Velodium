#include "Shader.h"

#include "src/Shader_edl.h"
#include "src/Shader_object.h"

#include "../Core/Dimension.h"



//Constructor / Destructor
Shader::Shader(Dimension* dimManager){
	//---------------------------

	this->edlManager = new Shader_edl(dimManager);

	//---------------------------
}
Shader::~Shader(){
	//---------------------------

	delete edlManager;
	delete shader_screen;
	delete shader_render;

	//---------------------------
}

void Shader::init_shader(){
	//---------------------------

	string path_screen_vs = "../src/Engine/Shader/glsh/shader_screen.vs";
	string path_screen_fs = "../src/Engine/Shader/glsh/shader_screen.fs";

	string path_render_vs = "../src/Engine/Shader/glsh/shader_edl.vs";
	string path_render_fs = "../src/Engine/Shader/glsh/shader_edl.fs";

	shader_screen = new Shader_object(path_screen_vs, path_screen_fs);
	shader_render = new Shader_object(path_render_vs, path_render_fs);

	edlManager->setup_edl(shader_render->get_program_ID());

	//---------------------------
}
void Shader::update_shader(){
	//---------------------------

	edlManager->setup_edl(shader_render->get_program_ID());

	//---------------------------
}
void Shader::use_shader(string shader_name){
	//---------------------------

	if(shader_name == "screen"){
		shader_screen->use();
	}
	else if(shader_name == "render"){
		shader_render->use();
	}

	//---------------------------
}
