#include "Shader.h"

#include "src/Shader_object.h"
#include "src/Shader_screen.h"
#include "src/Shader_edl.h"

#include "../Node_engine.h"



//Constructor / Destructor
Shader::Shader(Node_engine* node){
	//---------------------------

	this->edlManager = new Shader_edl(node);
	this->screenManager = new Shader_screen(node);

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

	string path_screen_vs = screenManager->get_path_vs();
	string path_screen_fs = screenManager->get_path_fs();

	string path_render_vs = edlManager->get_path_vs();
	string path_render_fs = edlManager->get_path_fs();

	this->shader_screen = new Shader_object(path_screen_vs, path_screen_fs);
	this->shader_render = new Shader_object(path_render_vs, path_render_fs);

	screenManager->setup_shader(shader_screen->get_program_ID());
	edlManager->setup_shader(shader_render->get_program_ID());

	//---------------------------
}
void Shader::update_shader(){
	//---------------------------

	screenManager->update_shader();
	edlManager->update_shader();

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
