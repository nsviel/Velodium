#ifndef OBJECT_AXIS_H
#define OBJECT_AXIS_H

#include "../../../common.h"
#include "../../Data/struct_glyph.h"
#include "../../Data/struct_subset.h"


class Axis
{
public:
  //Constructor / Destructor
  Axis();
  ~Axis();

public:
  Glyph* obj_axis();
  Glyph* obj_axis_circle(float circleRadius);

  void obj_axis_cloud(Subset* subset);
  void update_axis_cloud(Subset* subset);

private:

};

#endif
