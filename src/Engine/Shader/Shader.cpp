#include "Shader.h"

#include "Object/Shader_object.h"
#include "Object/Shader_source.h"
#include "Source/Shader_screen.h"
#include "Source/Shader_edl.h"

#include "../Node_engine.h"



//Constructor / Destructor
Shader::Shader(Node_engine* node){
	this->node_engine = node;
	//---------------------------

	this->list_shader_obj = new list<Shader_object*>();
	this->list_shader_src = new list<Shader_source*>();

	//---------------------------
}
Shader::~Shader(){}

//Main function
void Shader::init_shader(){
	//---------------------------

	// Screen shader
	Shader_screen* screenManager = new Shader_screen(node_engine);
	string path_screen_vs = screenManager->get_path_vs();
	string path_screen_fs = screenManager->get_path_fs();
	Shader_object* shader_screen = new Shader_object("screen", path_screen_vs, path_screen_fs);
	screenManager->setup_shader(shader_screen);
	this->list_shader_obj->push_back(shader_screen);
	this->list_shader_src->push_back(screenManager);

	//EDL shader
	Shader_edl* edlManager = new Shader_edl(node_engine);
	string path_render_vs = edlManager->get_path_vs();
	string path_render_fs = edlManager->get_path_fs();
	Shader_object* shader_render = new Shader_object("edl", path_render_vs, path_render_fs);
	edlManager->setup_shader(shader_render);
	this->list_shader_obj->push_back(shader_render);
	this->list_shader_src->push_back(edlManager);

	//Inversion shader
	string path_inv_vs = "../src/Engine/Shader/glsh/shader_inversion.vs";
	string path_inv_fs = "../src/Engine/Shader/glsh/shader_inversion.fs";
	Shader_object* shader_invers = new Shader_object("inversion", path_inv_vs, path_inv_fs);
	this->list_shader_obj->push_back(shader_invers);

	//---------------------------
}
void Shader::update_shader(){
	//---------------------------

	Shader_screen* screenManager = (Shader_screen*)get_shader_src_byName("screen");
	Shader_edl* edlManager = (Shader_edl*)get_shader_src_byName("edl");

	screenManager->update_shader();
	edlManager->update_shader();

	//---------------------------
}
void Shader::use_shader(string shader_name){
	//---------------------------

	//Retrieve shader object
	Shader_object* shader = get_shader_obj_byName(shader_name);

	//Use it
	if(shader != nullptr){
		shader->use();
	}

	//---------------------------
}

//Accesseur
Shader_object* Shader::get_shader_obj_byName(string querry){
	//---------------------------

	for(int i=0; i<list_shader_obj->size(); i++){
		Shader_object* shader_obj = *next(list_shader_obj->begin(), i);
		if(shader_obj->get_name() == querry){
			return shader_obj;
		}
	}

	//---------------------------
	return nullptr;
}
Shader_source* Shader::get_shader_src_byName(string querry){
	//---------------------------

	for(int i=0; i<list_shader_src->size(); i++){
		Shader_source* shader_src = *next(list_shader_src->begin(), i);
		if(shader_src->get_name() == querry){
			return shader_src;
		}
	}

	//---------------------------
	return nullptr;
}
