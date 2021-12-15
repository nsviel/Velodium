#ifndef OOBB_H
#define OOBB_H

#include "../../common.h"
#include "../Data/struct_glyph.h"


class OOBB
{
public:
  //Constructor / Destructor
  OOBB();
  ~OOBB();

public:
  Glyph* obj_oobb();

  void update_oobb(Glyph* obstacle, mat4 transformation);

  vector<vec3> build_box(vec3 min, vec3 max);

private:
  vec4 oobb_color;
};

#endif
