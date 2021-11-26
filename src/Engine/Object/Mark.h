#ifndef MARK_H
#define MARK_H

#include "../../common.h"


class Mark
{
public:
  //Constructor / Destructor
  Mark();
  ~Mark();

public:
  Glyph* obj_frameSelection();
  Glyph* obj_pointsAtLocation(vector<vec3>& pos);
  Glyph* obj_sphere_RGB(double r, int lats, int longs, vec3 pos, vec3 RGB_in);

private:
  vec4 selection_color;
};

#endif
