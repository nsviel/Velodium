#ifndef OBJECT_MARK_H
#define OBJECT_MARK_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"


class Mark
{
public:
  //Constructor / Destructor
  Mark();
  ~Mark();

public:
  void create_selection_frame();
  Glyph* obj_pointsAtLocation(vector<vec3>& pos);
  Glyph* obj_sphere_RGB(double r, int lats, int longs, vec3 pos, vec3 RGB_in);
  void update_selection_frame(vector<vec3> xyz);

  inline Glyph* get_selection_frame(){return selection_frame;}

private:
  Glyph* selection_frame;

  vec4 selection_frame_color;
};

#endif
