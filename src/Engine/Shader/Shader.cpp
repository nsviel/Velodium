#include "Shader.h"

#include "Base/Shader_obj.h"
#include "Base/Shader_src.h"
#include "Source/Shader_light.h"
#include "Source/Shader_edl.h"
#include "Source/Shader_pyramid.h"

#include "../Node_engine.h"

#include "../../Specific/Function/fct_error.h"


//Constructor / Destructor
Shader::Shader(Node_engine* node){
	this->node_engine = node;
	//---------------------------

	this->shader_dir = "../src/Engine/Shader/glsh/";
	this->list_shader_obj = new list<Shader_obj*>();
	this->list_shader_src = new list<Shader_src*>();

	//---------------------------
}
Shader::~Shader(){
	//---------------------------

	for(int i=0; i<list_shader_obj->size(); i++){
		Shader_obj* obj = *next(list_shader_obj->begin(), i);
		delete obj;
	}

	for(int i=0; i<list_shader_src->size(); i++){
		Shader_src* obj = *next(list_shader_src->begin(), i);
		delete obj;
	}

	//---------------------------
}

//Main function
void Shader::init_shader(){
	Shader_obj* shader;
	string path_vs, path_fs;
	//---------------------------

	// Untextured mesh shader
	path_vs = shader_dir + "pass_1/shader_mesh_untextured.vs";
	path_fs = shader_dir + "pass_1/shader_mesh_untextured.fs";
	shader = new Shader_obj("shader_mesh_untextured", path_vs, path_fs);
	this->list_shader_obj->push_back(shader);

	// Textured mesh shader
	path_vs = shader_dir + "pass_1/shader_mesh_textured.vs";
	path_fs = shader_dir + "pass_1/shader_mesh_textured.fs";
	shader = new Shader_obj("shader_mesh_textured", path_vs, path_fs);
	this->list_shader_obj->push_back(shader);

	// Light mesh shader
	path_vs = shader_dir + "pass_1/shader_lamp.vs";
	path_fs = shader_dir + "pass_1/shader_lamp.fs";
	Shader_obj* shader_lamp = new Shader_obj("shader_lamp", path_vs, path_fs);
	this->list_shader_obj->push_back(shader_lamp);

	//Recombinaison shader
	path_vs = shader_dir + "pass_2/shader_recombination.vs";
	path_fs = shader_dir + "pass_2/shader_recombination.fs";
	shader = new Shader_obj("shader_recombination", path_vs, path_fs);
	shader->use();
	shader->setInt("tex_color_1", 0);
	shader->setInt("tex_color_2", 1);
	shader->setInt("tex_depth_1", 2);
	shader->setInt("tex_depth_2", 3);
	this->list_shader_obj->push_back(shader);

	//Canvas shader
	path_vs = shader_dir + "pass_3/shader_canvas.vs";
	path_fs = shader_dir + "pass_3/shader_canvas.fs";
	shader = new Shader_obj("shader_canvas", path_vs, path_fs);
	this->list_shader_obj->push_back(shader);

	//Geometric shader
	path_vs = shader_dir + "pass_1/shader_geometric.vs";
	path_fs = shader_dir + "pass_1/shader_geometric.fs";
	shader = new Shader_obj("shader_geometry", path_vs, path_fs);
	this->list_shader_obj->push_back(shader);
	/*
	//Pyramid
	path_vs = shader_dir + "pyramid/shader_pyramid_lvl_0.vs";
	path_fs = shader_dir + "pyramid/shader_pyramid_lvl_0.fs";
	Shader_obj* shader_lvl_0 = new Shader_obj("shader_py_lvl_0", path_vs, path_fs);
	this->list_shader_obj->push_back(shader_lvl_0);

	path_vs = shader_dir + "pyramid/shader_pyramid_lvl_n.vs";
	path_fs = shader_dir + "pyramid/shader_pyramid_lvl_n.fs";
	Shader_obj* shader_lvl_n = new Shader_obj("shader_py_lvl_n", path_vs, path_fs);
	this->list_shader_obj->push_back(shader_lvl_n);

	path_vs = shader_dir + "pyramid/shader_pyramid_visibility.vs";
	path_fs = shader_dir + "pyramid/shader_pyramid_visibility.fs";
	Shader_obj* shader_pyramid_visibility = new Shader_obj("shader_py_visibility", path_vs, path_fs);
	this->list_shader_obj->push_back(shader_pyramid_visibility);

	Shader_pyramid* shader_pyramid = new Shader_pyramid(node_engine);
	shader_pyramid->setup_shader(shader_lvl_0, shader_lvl_n, shader_pyramid_visibility);
	this->list_shader_src->push_back(shader_pyramid);
	*/

	//EDL shader
	Shader_edl* shader_edl = new Shader_edl(node_engine);
	shader = new Shader_obj("shader_edl", shader_edl);
	shader_edl->setup_shader(shader);
	this->list_shader_obj->push_back(shader);
	this->list_shader_src->push_back(shader_edl);

	//Lighting shader
	path_vs = shader_dir + "experimental/shader_lighting.vs";
	path_fs = shader_dir + "experimental/shader_lighting.fs";
	Shader_obj* shader_lighting = new Shader_obj("shader_lighting", path_vs, path_fs);
	this->list_shader_obj->push_back(shader_lighting);
	Shader_light* lightManager = new Shader_light();
	list_shader_src->push_back(lightManager);
	lightManager->setup_shader(shader_lighting, shader_lamp);

	//---------------------------
}
void Shader::update_shader(){
	//---------------------------

	//EDL update
	Shader_src* shader_edl = get_shader_src_byName("render_edl");
	shader_edl->update_shader();

	//EDL update
	//Shader_src* shader_pyramid = get_shader_src_byName("pyramid");
	//shader_pyramid->update_shader();

	//...

	//---------------------------
}
void Shader::use_shader(string shader_name){
	//---------------------------

	//Retrieve shader object
	Shader_obj* shader_obj = get_shader_obj_byName(shader_name);

	//Use it
	if(shader_obj != nullptr){
		shader_obj->use();
	}else{
		cout<<"[error] Shader obj not found"<<endl;
	}

	//---------------------------
}

//Accesseur
Shader_obj* Shader::get_shader_obj_byName(string querry){
	//---------------------------

	if(list_shader_obj->size() == 0){
		cout << "[error] Shader src not being initialized !"<<endl;
	}

	//Search for corresponding shader and out it
	for(int i=0; i<list_shader_obj->size(); i++){
		Shader_obj* shader_obj = *next(list_shader_obj->begin(), i);
		if(shader_obj->get_name() == querry){
			return shader_obj;
		}
	}

	//If any shader is found, cry
	warning_shaderNotFound(querry);

	//---------------------------
	return nullptr;
}
Shader_src* Shader::get_shader_src_byName(string querry){
	//---------------------------

	//Search for corresponding shader and out it
	for(int i=0; i<list_shader_src->size(); i++){
		Shader_src* shader_src = *next(list_shader_src->begin(), i);
		if(shader_src->get_name() == querry){
			return shader_src;
		}
	}

	//If any shader is found, cry
	warning_shaderNotFound(querry);

	//---------------------------
	return nullptr;
}
