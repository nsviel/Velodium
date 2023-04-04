#include "Shader_pyramid.h"

#include "../../Node_engine.h"
#include "../../Core/Dimension.h"
#include "../../Core/Configuration.h"


Shader_pyramid::Shader_pyramid(Node_engine* node){
  //---------------------------

  this->dimManager = node->get_dimManager();
  this->configManager = node->get_configManager();

  this->clip_far = configManager->parse_json_f("camera", "clip_far");
  this->clip_near = configManager->parse_json_f("camera", "clip_near");

  //---------------------------
}
Shader_pyramid::~Shader_pyramid(){}

void Shader_pyramid::setup_shader(Shader_obj* shader_lvl_0, Shader_obj* shader_lvl_n, Shader_obj* shader_visibility){
  this->shader_lvl_0 = shader_lvl_0;
  this->shader_lvl_n = shader_lvl_n;
  this->shader_visibility = shader_visibility;
  //---------------------------

  //Get canvas dimension
  vec2 gl_dim = dimManager->get_win_dim();

  //Pyramid level 0
  shader_lvl_0->use();
  shader_lvl_0->setInt("tex_depth", 0);
	shader_lvl_0->setInt("tex_position", 1);
  shader_lvl_0->setFloat("Z_NEAR", clip_near);
  shader_lvl_0->setFloat("Z_FAR", clip_far);

  //Pyramid level n
  shader_lvl_n->use();
  shader_lvl_n->setInt("tex_depth", 0);
	shader_lvl_n->setInt("tex_position", 1);
  shader_lvl_n->setInt("GL_WIDTH", gl_dim.x);
  shader_lvl_n->setInt("GL_HEIGHT", gl_dim.y);

  //Pyramid visibility
  shader_visibility->use();
  shader_visibility->setInt("tex_depth", 0);
  shader_visibility->setInt("tex_color", 1);
  shader_visibility->setInt("tex_position", 2);
  shader_visibility->setInt("tex_coord_0", 3);
  shader_visibility->setInt("tex_coord_1", 5);
  shader_visibility->setInt("tex_coord_2", 6);
  shader_visibility->setInt("tex_coord_3", 7);
  shader_visibility->setInt("tex_coord_4", 8);
  shader_visibility->setInt("GL_WIDTH", gl_dim.x);
  shader_visibility->setInt("GL_HEIGHT", gl_dim.y);

  //---------------------------
}
void Shader_pyramid::update_shader(){
  //---------------------------

  //Get canvas dimension
  vec2 gl_dim = dimManager->get_win_dim();

  //Use corresponding shader program
  shader_lvl_n->use();
  shader_lvl_n->setFloat("Z_NEAR", clip_near);
  shader_lvl_n->setFloat("Z_FAR", clip_far);
  shader_lvl_n->setInt("GL_WIDTH", gl_dim.x);
  shader_lvl_n->setInt("GL_HEIGHT", gl_dim.y);
  shader_lvl_n->setInt("NN_SIZE", 2);

  //---------------------------
}
