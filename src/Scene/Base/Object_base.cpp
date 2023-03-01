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

  this->vao = -1;
  this->vbo_xyz = -1;
  this->vbo_rgb = -1;
  this->vbo_N = -1;

  this->has_color = false;
  this->has_normal = false;

  this->ID_perma = -1;
  this->ID_order = -1;

  this->name = "";
  this->format = "";
  this->path_file = "";
  this->path_save = "";
  this->draw_type = "";
  this->dataFormat = "";

  this->unicolor = glm::vec4(1.0f);
  this->visibility = true;
  this->nb_point = 0;

  this->min = glm::vec3(0.0f);
  this->max = glm::vec3(0.0f);
  this->root = glm::vec3(0.0f);
  this->COM = glm::vec3(0.0f);
  this->rotat = glm::mat4(1.0f);
  this->trans = glm::mat4(1.0f);
  this->scale = glm::mat4(1.0f);
  this->transformation = glm::mat4(1.0f);

  //---------------------------
}
void Object_base::reset_object(){
  //---------------------------

  this->min = glm::vec3(0.0f);
  this->max = glm::vec3(0.0f);
  this->root = glm::vec3(0.0f);
  this->COM = glm::vec3(0.0f);
  this->rotat = glm::mat4(1.0f);
  this->trans = glm::mat4(1.0f);
  this->scale = glm::mat4(1.0f);
  this->transformation = glm::mat4(1.0f);

  //---------------------------
}

void Object_base::draw(){
  //---------------------------

  if(visibility){
    glBindVertexArray(vao);
    std::cout<<xyz.size()<<std::endl;
    glDrawArrays(GL_POINTS, 0, xyz.size());
    glBindVertexArray(0);
  }

  //---------------------------
}
