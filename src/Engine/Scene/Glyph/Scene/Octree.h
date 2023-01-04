#ifndef OBJECT_OCTREE_H
#define OBJECT_OCTREE_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"

struct Cube{
  vector<vec3> xyz;
  vector<vec4> rgb;
  vector<Cube*> child;
  list<int> idx;
  int level;
  vec3 min;
  vec3 max;
  vec3 center;
};
struct Tree{
  vector<vec3> xyz;
  vector<vec4> rgb;
  Cube* root;
  vec3 min;
  vec3 max;
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
  vector<vec3> compute_cube_location(vec3 min, vec3 max);
  vector<vec4> compute_cube_color(int size);
  void compute_cube_division(Tree& tree, Cube* cube, vector<vec3>& point);

  inline Glyph* get_glyph(){return octree;}

private:
  Glyph* octree;
  vec4 octree_color;
  int nb_cell;
};

#endif
