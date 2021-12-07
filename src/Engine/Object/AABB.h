#ifndef AABB_H
#define AABB_H

#include "../../common.h"


class AABB
{
public:
  //Constructor / Destructor
  AABB();
  ~AABB();

public:
  Glyph* obj_aabb();
  Glyph* obj_obstacle();

  void update_aabb(Glyph* aabb, Cloud* cloud);
  void update_aabb(Glyph* aabb, Subset* subset);
  void update_obstacle(Glyph* obstacle, mat4 transformation);

  vector<vec3> build_box(vec3 min, vec3 max);

private:
  vec4 aabb_color;
  vec4 obstacle_color;
};

#endif
