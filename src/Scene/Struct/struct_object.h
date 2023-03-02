#ifndef OBJECT_STRUCT_H
#define OBJECT_STRUCT_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>


struct Object_{ //Cloud subset / part
  //---------------------------

  //OpenGL
  GLuint vao;
  GLuint vbo_xyz;
  GLuint vbo_rgb;
  GLuint vbo_Nxyz;
  GLuint vbo_tex;

  //Drawing
  int draw_type;
  int draw_point_size;
  int draw_line_width;
  std::string draw_type_name;

  //Infos
  int ID;
  int nb_point;
  std::string name;
  std::string path;
  glm::vec4 unicolor;
  int texture_ID;

  bool visibility;
  bool has_color;
  bool has_intensity;
  bool has_normal;
  bool has_texture;

  //Data
  std::vector<glm::vec3> xyz;
  std::vector<glm::vec4> rgb;
  std::vector<glm::vec3> Nxyz;
  std::vector<glm::vec2> uv;
  std::vector<float> ts;

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
