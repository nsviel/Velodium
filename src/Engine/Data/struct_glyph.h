#ifndef GLYPH_STRUCT_H
#define GLYPH_STRUCT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

/**
 * \brief Glyph structure storing all associated data
 * \struct Glyph struct_glyph.h "Glyph structure"
 */
struct Glyph{
  //---------------------------

  GLuint VAO;
  GLuint VBO_location;
  GLuint VBO_color;
  GLuint VBO_normal;

  int ID;
  float draw_width;
  float draw_size;
  bool visibility;
  bool permanent;
  std::string name;
  std::string draw_type;

  glm::vec3 COM;
  glm::vec3 max;
  glm::vec3 min;
  glm::vec4 color_unique;
  std::vector<glm::vec3> location;
  std::vector<glm::vec3> location_init;
  std::vector<glm::vec3> normal;
  std::vector<glm::vec4> color;
  std::vector<float> timestamp;

  //---------------------------
};

#endif
