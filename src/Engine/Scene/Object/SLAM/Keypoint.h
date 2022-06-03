#ifndef OBJECT_KEYPOINT_H
#define OBJECT_KEYPOINT_H

#include "../../../Data/struct_glyph.h"

#include "../../../../common.h"


class Keypoint
{
public:
  //Constructor / Destructor
  Keypoint();
  ~Keypoint();

public:
  void create_keypoint(Subset* subset);
  void update_keypoint_location(Subset* subset);
  void update_keypoint_normal(Subset* subset);

  inline void set_visibility(bool value){this->visibility = value;}
  inline bool* get_visibility(){return &visibility;}

private:
  Glyph* keypoint;
  bool visibility;
  vec4 color;
};

#endif
