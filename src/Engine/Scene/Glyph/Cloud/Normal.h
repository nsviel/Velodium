#ifndef OBJECT_NORMAL_H
#define OBJECT_NORMAL_H

#include "../../../Data/struct_glyph.h"
#include "../../../Data/struct_subset.h"

#include "../../../../common.h"


class Normal
{
public:
  //Constructor / Destructor
  Normal();
  ~Normal();

public:
  void create_normal_subset(Subset* subset);
  void update_normal_subset(Subset* subset);

  inline bool* get_visibility(){return &visibility;}
  inline int* get_size(){return &size;}

private:
  bool visibility;
  vec4 color;
  int size;
  int width;
};

#endif
