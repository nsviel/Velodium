#include "Shader.h"

#include "Base/Shader_object.h"
#include "Base/Shader_source.h"
#include "Source/Shader_screen.h"
#include "Source/Shader_edl.h"

#include "../Node_engine.h"

#include "../../Specific/Function/fct_error.h"


//Constructor / Destructor
Shader::Shader(Node_engine* node){
	this->node_engine = node;
	//---------------------------

	this->shader_dir = "../src/Engine/Shader/glsh/";
	this->list_shader_obj = new list<Shader_object*>();
	this->list_shader_src = new list<Shader_source*>();

	//---------------------------
}
Shader::~Shader(){}

//Main function
void Shader::init_shader_objects(){
	Shader_object* shader;
	string path_vs, path_fs;
	//---------------------------

	// Screen shader
	path_vs = shader_dir + "standard/shader_screen.vs";
	path_fs = shader_dir + "standard/shader_screen.fs";
	shader = new Shader_object("screen", path_vs, path_fs);
	this->list_shader_obj->push_back(shader);

	// Texture shader
	path_vs = shader_dir + "standard/shader_texture.vs";
	path_fs = shader_dir + "standard/shader_texture.fs";
	shader = new Shader_object("texture", path_vs, path_fs);
	this->list_shader_obj->push_back(shader);

	//EDL shader
	Shader_edl* edlManager = new Shader_edl(node_engine);
	shader = new Shader_object(edlManager);
	edlManager->setup_shader(shader);
	this->list_shader_obj->push_back(shader);
	this->list_shader_src->push_back(edlManager);

	//Inversion shader
	path_vs = shader_dir + "experimental/shader_inversion.vs";
	path_fs = shader_dir + "experimental/shader_inversion.fs";
	shader = new Shader_object("inversion", path_vs, path_fs);
	this->list_shader_obj->push_back(shader);

	//---------------------------
}
void Shader::update_shader_objects(){
	//---------------------------

	//Shader_source* shader_screen = get_shader_src_byName("screen");
	//shader_screen->update_shader();

	Shader_source* shader_edl = get_shader_src_byName("edl");
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

	//Search for corresponding shader and out it
	for(int i=0; i<list_shader_obj->size(); i++){
		Shader_object* shader_obj = *next(list_shader_obj->begin(), i);
		if(shader_obj->get_name() == querry){
			return shader_obj;
		}
	}

	//If any shader is found, cry
	warning_shaderNotFound(querry);

	//---------------------------
	return nullptr;
}
Shader_source* Shader::get_shader_src_byName(string querry){
	//---------------------------

	//Search for corresponding shader and out it
	for(int i=0; i<list_shader_src->size(); i++){
		Shader_source* shader_src = *next(list_shader_src->begin(), i);
		if(shader_src->get_name() == querry){
			return shader_src;
		}
	}

	//If any shader is found, cry
	warning_shaderNotFound(querry);

	//---------------------------
	return nullptr;
}
