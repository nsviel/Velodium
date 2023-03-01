#include "Object_base.h"


//Constructor / Destructor
Object_base::Object_base(){
  //---------------------------

  this->init_object();

  //---------------------------
}
Object_base::~Object_base(){}

//Main function
void Object_base::init_object(){
  //---------------------------

  glGenVertexArrays(1, &vao);
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
  this->draw_type_name = "point";
  this->draw_type = GL_POINTS;
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

//Draw stuff
void Object_base::draw(){
  //---------------------------

  if(visibility){
    glBindVertexArray(vao);
    glDrawArrays(draw_type, 0, xyz.size());
    glBindVertexArray(0);
  }

  //---------------------------
}
void Object_base::set_draw_type(std::string name){
  //---------------------------

  if(name == "point"){
    this->draw_type = GL_POINTS;
  }
  else if(name == "line"){
    this->draw_type = GL_LINES;
  }
  else if(name == "triangle"){
    this->draw_type = GL_TRIANGLES;
  }
  else if(name == "triangle_strip"){
    this->draw_type = GL_TRIANGLE_STRIP;
  }
  else if(name == "triangle_fan"){
    this->draw_type = GL_TRIANGLE_FAN;
  }
  else if(name == "quad"){
    this->draw_type = GL_QUADS;
  }
  else if(name == "quad_strip"){
    this->draw_type = GL_QUAD_STRIP;
  }
  else if(name == "polygon"){
    this->draw_type = GL_POLYGON;
  }
  else{
    this->draw_type = GL_POINTS;
  }

  //---------------------------
}

//Insert data
void Object_base::insert_location(std::vector<glm::vec3>& xyz_in){
  this->xyz = xyz_in;
  //---------------------------

  glBindVertexArray(vao);
  glGenBuffers(1, &vbo_xyz);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_xyz);
  glBufferData(GL_ARRAY_BUFFER, xyz.size()*sizeof(glm::vec3), &xyz[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glBindVertexArray(0);

  //---------------------------
}
void Object_base::insert_color(std::vector<glm::vec4>& rgb_in){
  this->rgb = rgb_in;
  //---------------------------

  glBindVertexArray(vao);
  glGenBuffers(1, &vbo_xyz);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_xyz);
  glBufferData(GL_ARRAY_BUFFER, rgb.size()*sizeof(glm::vec4), &rgb[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glBindVertexArray(0);

  //---------------------------
}
