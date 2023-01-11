#ifndef OBJECT_MATCHING_H
#define OBJECT_MATCHING_H

#include "../../../Data/struct_glyph.h"
#include "../../../Data/struct_subset.h"

#include "../../../../common.h"


class Matching
{
public:
  //Constructor / Destructor
  Matching();
  ~Matching();

public:
  void create_matching();
  void update_matching(vector<vec3>& xyz);
  void reset();

  inline Glyph* get_glyph(){return matching;}

private:
  Glyph* matching;
  vec4 color;
};

#endif
