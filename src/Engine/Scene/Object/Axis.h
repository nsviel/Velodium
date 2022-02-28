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
  void create_axis_scene();
  void create_axis_circle(float circleRadius);
  void create_axis_subset(Subset* subset);
  void update_axis_subset(Subset* subset);

  inline Glyph* get_axis_scene(){return axis_scene;}
  inline Glyph* get_axis_circle(){return axis_circle;}

private:
  Glyph* axis_scene;
  Glyph* axis_circle;
};

#endif
