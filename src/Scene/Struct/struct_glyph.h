#ifndef GLYPH_STRUCT_H
#define GLYPH_STRUCT_H

#include "struct_object.h"


struct Glyph : public Object_
{
  //---------------------------

  bool permanent;
  glm::vec4 color_unique;
  std::vector<glm::vec3> xyz_init;

  //---------------------------
};

#endif
