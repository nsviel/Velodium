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
void Shader::init_shader_objects(){
	//---------------------------

	// Screen shader
	Shader_screen* screenManager = new Shader_screen(node_engine);
	Shader_object* shader_screen = new Shader_object(screenManager);
	screenManager->setup_shader(shader_screen);
	this->list_shader_obj->push_back(shader_screen);
	this->list_shader_src->push_back(screenManager);

	//EDL shader
	Shader_edl* edlManager = new Shader_edl(node_engine);
	Shader_object* shader_render = new Shader_object(edlManager);
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
void Shader::update_shader_objects(){
	//---------------------------

	Shader_source* shader_screen = get_shader_src_byName("screen");
	Shader_source* shader_edl = get_shader_src_byName("edl");

	shader_screen->update_shader();
	shader_edl->update_shader();

	//---------------------------
}
void Shader::use_shader(string shader_name){
	//---------------------------

	//Retrieve shader object
	Shader_object* shader_obj = get_shader_obj_byName(shader_name);

	//Use it
	if(shader_obj != nullptr){
		shader_obj->use();
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
