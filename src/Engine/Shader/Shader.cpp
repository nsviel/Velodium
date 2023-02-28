#include "Shader.h"

#include "Object/Shader_object.h"
#include "Object/Shader_source.h"
#include "Source/Shader_screen.h"
#include "Source/Shader_edl.h"

#include "../Node_engine.h"



//Constructor / Destructor
Shader::Shader(Node_engine* node){
	//---------------------------

	this->edlManager = new Shader_edl(node);
	this->screenManager = new Shader_screen(node);

	this->list_shader_obj = new list<Shader_object*>();
	this->list_shader_src = new list<Shader_source*>();

	list_shader_src->push_back(edlManager);

	//---------------------------
}
Shader::~Shader(){
	//---------------------------

	delete edlManager;

	//---------------------------
}

void Shader::init_shader(){
	//---------------------------

	// Screen shader
	string path_screen_vs = screenManager->get_path_vs();
	string path_screen_fs = screenManager->get_path_fs();
	Shader_object* shader_screen = new Shader_object("screen", path_screen_vs, path_screen_fs);
	screenManager->setup_shader(shader_screen);
	this->list_shader_obj->push_back(shader_screen);

	//EDL shader
	string path_render_vs = edlManager->get_path_vs();
	string path_render_fs = edlManager->get_path_fs();
	Shader_object* shader_render = new Shader_object("edl", path_render_vs, path_render_fs);
	edlManager->setup_shader(shader_render);
	this->list_shader_obj->push_back(shader_render);

	//Inversion shader
	string path_inv_vs = "../src/Engine/Shader/glsh/shader_inversion.vs";
	string path_inv_fs = "../src/Engine/Shader/glsh/shader_inversion.fs";
	Shader_object* shader_invers = new Shader_object("inversion", path_inv_vs, path_inv_fs);
	this->list_shader_obj->push_back(shader_invers);

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

	//Retrieve shader object
	Shader_object* shader = get_shader_byName(shader_name);

	//Use it
	if(shader != nullptr){
		shader->use();
	}

	//---------------------------
}
Shader_object* Shader::get_shader_byName(string querry_name){
	//---------------------------

	for(int i=0; i<list_shader_obj->size(); i++){
		Shader_object* shader = *next(list_shader_obj->begin(), i);
		if(shader->get_name() == querry_name){
			return shader;
		}
	}

	//---------------------------
	return nullptr;
}
