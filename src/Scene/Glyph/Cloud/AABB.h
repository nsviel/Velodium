#ifndef OBJECT_AABB_H
#define OBJECT_AABB_H

#include "../../Base/Object_base.h"


class AABB : public Object_base
{
public:
  AABB();

public:
  void create_glyph();
  void update_glyph(Cloud* cloud);
  void update_glyph(Subset* subset);
  vector<vec3> build_box(vec3 min, vec3 max);

private:
  int width;
};

#endif