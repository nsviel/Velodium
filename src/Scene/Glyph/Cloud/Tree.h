#ifndef OBJECT_TREE_H
#define OBJECT_TREE_H

#include "../Base/Glyph_source.h"

class Octree;


class Tree : public Glyph_source
{
public:
  //Constructor / Destructor
  Tree();
  ~Tree();

public:
  void create_glyph(Subset* subset);
  void update_tree(Subset* subset);

  inline bool* get_visibility(){return &visibility;}
  inline int* get_tree_level(){return &tree_level;}

private:
  Octree* octreeManager;
  bool visibility;
  vec4 tree_color;
  int tree_level;
};

#endif
