#ifndef OBJECT_TREE_H
#define OBJECT_TREE_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"

class Octree;


class Tree
{
public:
  //Constructor / Destructor
  Tree();
  ~Tree();

public:
  void create_tree(Subset* subset);
  void update_tree(Subset* subset);

private:
  Octree* octreeManager;
  vec4 tree_color;
};

#endif
