#ifndef OBJECT_GRID_H
#define OBJECT_GRID_H

#include "../../../common.h"
#include "../../Data/struct_glyph.h"


class Grid
{
public:
  //Constructor / Destructor
  Grid();
  ~Grid();

public:
  void create_grid();
  void create_grid_sub();
  void create_grid_plane();

  inline Glyph* get_grid(){return grid;}
  inline Glyph* get_grid_sub(){return grid_sub;}
  inline Glyph* get_grid_plane(){return grid_plane;}
  inline vec4* get_grid_color(){return &grid_color;}

private:
  Glyph* grid;
  Glyph* grid_sub;
  Glyph* grid_plane;
  vec4 grid_color;
  int nb_cell;
};

#endif
