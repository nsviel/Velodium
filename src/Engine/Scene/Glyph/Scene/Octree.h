#ifndef OBJECT_OCTREE_H
#define OBJECT_OCTREE_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"


class Octree
{
public:
  //Constructor / Destructor
  Octree();
  ~Octree();

public:
  void create_octree();
  void update_octree(int value);

  inline Glyph* get_octree(){return octree;}

private:
  Glyph* octree;
  vec4 octree_color;
  int nb_cell;
};

#endif
