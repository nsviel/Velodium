#ifndef OBJECT_STRUCT_H
#define OBJECT_STRUCT_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>


struct Object_{ //Cloud cloud / part
  //---------------------------

  Object_();

  //OpenGL
  GLuint vao;
  GLuint vbo_xyz;
  GLuint vbo_rgb;
  GLuint vbo_Nxyz;
  GLuint vbo_uv;

  //Drawing
  int draw_type;
  int draw_point_size;
  int draw_line_width;

  //Infos
  int ID;
  int nb_point;
  std::string name;
  std::string path_file;
  std::string path_save;
  std::string file_format;
  std::string draw_type_name;
  std::string obj_type;
  glm::vec4 unicolor;

  bool is_visible;
  bool has_color;
  bool has_intensity;
  bool has_normal;
  bool has_texture;
  bool has_timestamp;

  //Data
  std::vector<glm::vec3> xyz;
  std::vector<glm::vec4> rgb;
  std::vector<glm::vec3> Nxyz;
  std::vector<glm::vec2> uv;
  std::vector<float> ts;
  std::vector<int> tex_ID;

  //Pose
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 root;
  glm::vec3 COM;
  glm::mat4 rotat;
  glm::mat4 trans;
  glm::mat4 scale;
  glm::mat4 transformation;

  //---------------------------
};

#endif
