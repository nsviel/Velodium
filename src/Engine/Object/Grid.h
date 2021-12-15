#ifndef GRID_H
#define GRID_H

#include "../../common.h"
#include "../Data/struct_glyph.h"


class Grid
{
public:
  //Constructor / Destructor
  Grid();
  ~Grid();

public:
  Glyph* obj_grid();
  Glyph* obj_subgrid();
  Glyph* obj_planegrid();

private:
  vec4 grid_color;
  int nb_cell;
};

#endif
