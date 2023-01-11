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

  // GLyph info & parameters
  int ID;
  float draw_width;
  float draw_size;
  bool visibility;
  bool permanent;
  std::string name;
  std::string draw_type;

  // Data indicator
  glm::vec3 COM;
  glm::vec3 max;
  glm::vec3 min;
  glm::vec4 color_unique;

  // Data
  std::vector<glm::vec3> xyz;
  std::vector<glm::vec3> xyz_init;
  std::vector<glm::vec4> rgb;
  std::vector<glm::vec3> Nxyz;
  std::vector<float> ts;

  // OpenGL stuff
  GLuint VAO;
  GLuint VBO_xyz;
  GLuint VBO_rgb;
  GLuint VBO_normal;

  //---------------------------
};

#endif
