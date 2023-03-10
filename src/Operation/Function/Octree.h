#ifndef OCTREE_H
#define OCTREE_H

#include "../../Scene/Base/struct_glyph.h"
#include "../../common.h"


struct Cube{
  int level;
  Cube* child[8];

  vec3 min;
  vec3 max;
  vec3 center;

  vector<int> idx_cube;
  vector<int> idx_child;
};
struct Root{
  Cube* child;

  vector<vec3>* xyz_subset;
  vector<vec3> xyz;
  vector<vec4> rgb;
  vector<vec4> level_rgb;
};


class Octree
{
public:
  //Constructor / Destructor
  Octree();
  ~Octree();

public:
  void create_octree(Cloud* cloud);
  void create_octree(Cloud* cloud, int level);
  void remove_octree(Root* root);

  //Sub functions
  void remove_cube(Cube* cube);
  void build_root(Cloud* cloud);
  void build_octree(Cube* cube_parent);
  vector<vec3> compute_cube_location(vec3 min, vec3 max);
  vector<vec4> compute_cube_color(int size);
  vector<vec4> compute_cube_color(int size, vec4 rgb);
  void compute_cube_division(Cube* cube);
  vector<int> compute_idx_from_point(vec3 min, vec3 max, Cube* cube_parent);
  bool is_at_least_one_value(vector<int> vec);

private:
  Root* root;
  vec4 octree_color;
  bool with_rdm_color;
  int octree_time;
  int nb_level;
};

#endif
