#ifndef OBJECT_CAR_H
#define OBJECT_CAR_H

#include "../../Base/Object_base.h"

class Transformation;


class Car : public Object_base
{
public:
  Car();

public:
  void create_glyph();
  void update_glyph(Cloud* cloud);
  void reset_glyph();

private:
  Transformation* transformManager;

  float lidar_height;
  int width;
};

#endif
