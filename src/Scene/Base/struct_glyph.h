#ifndef GLYPH_STRUCT_H
#define GLYPH_STRUCT_H

#include "struct_object.h"


struct Glyph : public Object_
{
  //---------------------------

  Glyph();

  bool is_permanent;
  std::vector<glm::vec3> xyz_init;
  Object_* linked_object;

  //---------------------------
};

#endif
