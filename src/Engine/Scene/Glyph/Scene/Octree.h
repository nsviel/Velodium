#ifndef OBJECT_OCTREE_H
#define OBJECT_OCTREE_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"

struct Cube{
  int level;
  Cube* child[8];

  vec3 min;
  vec3 max;
  vec3 center;

  vector<int> idx_cube;
  vector<int> idx_child;
};
struct Tree{
  Cube* root;

  vector<vec3>* xyz_subset;
  vector<vec3> xyz;
  vector<vec4> rgb;
};


class Octree
{
public:
  //Constructor / Destructor
  Octree();
  ~Octree();

public:
  void create_octree();
  void update_octree(Subset* subset);

  //Sub functions
  void build_octree(Tree& tree, Cube* cube_parent, int level_max);
  vector<vec3> compute_cube_location(vec3 min, vec3 max);
  vector<vec4> compute_cube_color(int size);
  vector<vec4> compute_cube_color(int size, vec4 rgb);
  void compute_cube_division(Tree& tree, Cube* cube);
  vector<int> compute_idx_from_point(Tree& tree, vec3 min, vec3 max, Cube* cube_parent);

  inline Glyph* get_glyph(){return octree;}

private:
  Glyph* octree;
  vec4 octree_color;
  int nb_cell;
};

#endif
