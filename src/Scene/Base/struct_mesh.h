#ifndef MESH_STRUCT_H
#define MESH_STRUCT_H

#include "struct_object.h"


struct Mesh : public Object_
{
  //---------------------------

  std::vector<glm::vec2> uv;
  std::vector<int> tex_ID;

  //---------------------------
};

#endif
