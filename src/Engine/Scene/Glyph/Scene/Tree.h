#ifndef OBJECT_TREE_H
#define OBJECT_TREE_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"


class Tree
{
public:
  //Constructor / Destructor
  Tree();
  ~Tree();

public:
  void create_tree();
  void update_tree(Subset* subset);

  inline Glyph* get_glyph(){return tree;}

private:
  Glyph* tree;
  vec4 tree_color;
};

#endif
