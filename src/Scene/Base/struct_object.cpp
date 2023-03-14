#include "struct_object.h"


Object_::Object_(){
  //---------------------------

  //Drawing
  this->draw_type = GL_POINTS;
  this->draw_point_size = 1;
  this->draw_line_width = 1;
  this->draw_type_name = "point";
  this->obj_type = "object";

  //Infos
  this->ID = -1;
  this->nb_point = 0;
  this->name = "";
  this->unicolor = glm::vec4(1.0f);

  this->is_visible = true;
  this->has_color = false;
  this->has_intensity = false;
  this->has_normal = false;
  this->has_texture = false;

  this->path_file = "";
  this->path_save = "";
  this->file_format = "";

  //Pose
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
