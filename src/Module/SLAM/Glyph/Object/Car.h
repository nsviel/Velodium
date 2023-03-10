#ifndef SLAM_OBJECT_CAR_H
#define SLAM_OBJECT_CAR_H

#include "../../../../Scene/Glyph/Base/Glyph_source.h"


class Car : public Glyph_source
{
public:
  Car();

public:
  void create_glyph();
  void update_glyph(Collection* collection);
  void reset_glyph();

private:
  float lidar_height;
  int width;
};

#endif
