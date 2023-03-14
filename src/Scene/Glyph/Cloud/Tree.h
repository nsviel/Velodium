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
  Glyph* create_glyph(Cloud* cloud);
  void update_tree(Cloud* cloud, Glyph* glyph);

  inline bool* get_visibility(){return &is_visible;}
  inline int* get_tree_level(){return &tree_level;}

private:
  Octree* octreeManager;
  bool is_visible;
  vec4 tree_color;
  int tree_level;
};

#endif
