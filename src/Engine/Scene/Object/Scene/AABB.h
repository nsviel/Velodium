#ifndef OBJECT_AABB_H
#define OBJECT_AABB_H

#include "../../../Data/struct_glyph.h"
#include "../../../Data/struct_cloud.h"

#include "../../../../common.h"


class AABB
{
public:
  //Constructor / Destructor
  AABB();
  ~AABB();

public:
  void create_aabb_scene();
  void update_aabb(Cloud* cloud);
  void update_aabb(Subset* subset);
  vector<vec3> build_box(vec3 min, vec3 max);

  inline Glyph* get_aabb(){return aabb;}
  inline vec4* get_aabb_color(){return &color;}

private:
  Glyph* aabb;

  vec4 color;
  int width;
};

#endif
