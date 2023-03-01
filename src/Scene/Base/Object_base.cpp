#include "Object_base.h"


//Constructor / Destructor
Object_base::Object_base(){
  //---------------------------

  this->init_object();

  //---------------------------
}
Object_base::~Object_base(){}

void Object_base::init_object(){
  //---------------------------

  this->VAO = -1;
  this->VBO_xyz = -1;
  this->VBO_rgb = -1;
  this->VBO_N = -1;

  this->has_color = false;
  this->has_normal = false;

  this->ID_perm = -1;
  this->ID_order = -1;

  this->name = "";
  this->format = "";
  this->path_file = "";
  this->path_save = "";
  this->draw_type = "";
  this->dataFormat = "";

  this->unicolor = vec4(1.0f);
  this->visibility = false;
  this->nb_point = 0;

  this->min = vec3(0.0f);
  this->max = vec3(0.0f);
  this->root = vec3(0.0f);
  this->COM = vec3(0.0f);
  this->rotat = mat4(1.0f);
  this->trans = mat4(1.0f);
  this->scale = mat4(1.0f);
  this->transformation = mat4(1.0f);

  //---------------------------
}
void Object_base::reset_object(){
  //---------------------------

  this->min = vec3(0.0f);
  this->max = vec3(0.0f);
  this->root = vec3(0.0f);
  this->COM = vec3(0.0f);
  this->rotat = mat4(1.0f);
  this->trans = mat4(1.0f);
  this->scale = mat4(1.0f);
  this->transformation = mat4(1.0f);

  //---------------------------
}
