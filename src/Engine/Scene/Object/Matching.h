#ifndef OBJECT_MATCHING_H
#define OBJECT_MATCHING_H

#include "../../../common.h"
#include "../../Data/struct_glyph.h"


class Matching
{
public:
  //Constructor / Destructor
  Matching();
  ~Matching();

public:
  void create_keypoint_subset(Subset* subset);
  void update_keypoint_subset(Subset* subset);

  inline void set_visibility(bool value){this->visibility = value;}
  inline bool* get_visibility(){return &visibility;}

private:
  Glyph* keypoint;
  bool visibility;
  vec4 color;
};

#endif
