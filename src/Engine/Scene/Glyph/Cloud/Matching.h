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
  void create_matching_subset(Subset* subset);
  void update_matching_subset(Subset* subset);

  inline Glyph* get_glyph(){return matching;}
  inline bool* get_visibility(){return &visibility;}

private:
  Glyph* matching;
  bool visibility;
  vec4 color;
};

#endif
