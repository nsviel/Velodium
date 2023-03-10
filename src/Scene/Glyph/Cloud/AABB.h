#ifndef OBJECT_AABB_H
#define OBJECT_AABB_H

#include "../Base/Glyph_source.h"


class AABB : public Glyph_source
{
public:
  AABB();

public:
  void create_glyph();
  void update_glyph(Collection* collection, Glyph* glyph);
  void update_glyph(Cloud* cloud);
  vector<vec3> build_box(vec3 min, vec3 max);

private:
  int width;
};

#endif
