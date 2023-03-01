#ifndef MESH_BASE_H
#define MESH_BASE_H

#include "Object_base.h"


class Mesh_base : public Object_base
{
public:
  //Constructor / Destructor
  Mesh_base();
  ~Mesh_base();

public:

private:
  //Texture
  int texture_ID;
  std::vector<glm::vec2> uv;
  bool has_texture;
};

#endif
