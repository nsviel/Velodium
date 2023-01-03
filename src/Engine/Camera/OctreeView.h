#ifndef OCTREE_H
#define OCTREE_H

#include "../../common.h"


class OctreeView
{
public:
  OctreeView();
  ~OctreeView();

public:
  //Main functions
  void construct_octree(Subset* subset);

private:
  vector<vec3> xyz;
  vector<vec4> rgb;
};

#endif
