#ifndef OBJECT_BOX_H
#define OBJECT_BOX_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"


class Box
{
public:
  //Constructor / Destructor
  Box();
  ~Box();

public:
  //Main function
  void create_box();
  void update_box();

  //Subfunction
  void build_box_location();
  void build_box_color();
  void compute_box_MinMax(Subset* subset, vec3 min_perc, vec3 max_perc);

  inline Glyph* get_glyph(){return box;}

private:
  Glyph* box;

  vec4 color;
  int width;
};

#endif
