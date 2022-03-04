#ifndef OBJECT_OOBB_H
#define OBJECT_OOBB_H

#include "../../../common.h"
#include "../../Data/struct_glyph.h"


class OOBB
{
public:
  //Constructor / Destructor
  OOBB();
  ~OOBB();

public:
  Glyph* create_oobb();
  void update_oobb(Glyph* oobb, mat4 transformation);
  vector<vec3> build_box(vec3 min, vec3 max);

  inline Glyph* get_oobb(){return oobb;}

private:
  Glyph* oobb;
  vec4 color;
  int width;
};

#endif
