#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "../Struct/struct_glyph.h"
#include "../Struct/struct_subset.h"

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <GL/gl.h>
#include <glm/glm.hpp>


class Object_base
{
public:
  //Constructor / Destructor
  Object_base();
  ~Object_base();

public:
  //Main function
  void init_object();
  void reset_object();

  //Draw stuff
  void draw();
  void set_draw_type(std::string name);

  //Insert data
  void insert_location(std::vector<glm::vec3>& xyz);
  void insert_color(std::vector<glm::vec4>& rgb_in);

public:
  //Generic data
  GLuint vao;
  GLuint vbo_xyz;
  GLuint vbo_rgb;
  GLuint vbo_N;

  std::vector<glm::vec3> xyz;
  std::vector<glm::vec4> rgb;
  std::vector<glm::vec3> N;

  bool has_color;
  bool has_normal;

  //Generic info
  GLuint ID_perma; //Permanent ID
  GLuint ID_order; // List order ID

  std::string name;
  std::string format;
  std::string object_type;
  std::string path_file;
  std::string path_save;
  std::string draw_type_name;
  std::string dataFormat;
  glm::vec4 unicolor;
  bool visibility;
  int nb_point;
  int draw_type;

  //Generic pose
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 root;
  glm::vec3 COM;
  glm::mat4 rotat;
  glm::mat4 trans;
  glm::mat4 scale;
  glm::mat4 transformation;

  //Attached glyphs
  std::map<std::string, Glyph> glyphs;
};


#endif
