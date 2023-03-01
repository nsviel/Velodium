#ifndef OBJECT_OOBB_H
#define OBJECT_OOBB_H

#include "../Base/Glyph_source.h"


class OOBB : public Glyph_source
{
public:
  //Constructor / Destructor
  OOBB();
  ~OOBB();

public:
  void create_glyph();
  void update_glyph(Glyph* glyph, mat4 transformation);
  vector<vec3> build_box(vec3 min, vec3 max);

private:
  Glyph* glyph;
  vec4 color;
  int width;
};

#endif
