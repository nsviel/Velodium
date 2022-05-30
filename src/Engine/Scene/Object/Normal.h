#ifndef OBJECT_NORMAL_H
#define OBJECT_NORMAL_H

#include "../../../common.h"
#include "../../Data/struct_glyph.h"
#include "../../Data/struct_subset.h"


class Normal
{
public:
  //Constructor / Destructor
  Normal();
  ~Normal();

public:
  void create_normal_subset(Subset* subset);
  void update_normal_subset(Subset* subset);

  inline Glyph* get_glyph(){return normal;}
  inline bool* get_visibility(){return &visibility;}

private:
  Glyph* normal;
  bool visibility;
  vec4 color;
  int width;
};

#endif
