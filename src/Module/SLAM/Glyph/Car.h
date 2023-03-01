#ifndef SLAM_OBJECT_CAR_H
#define SLAM_OBJECT_CAR_H

#include "../Base/SLAM_object_base.h"


class Car : public SLAM_object_base
{
public:
  Car();

public:
  void create_glyph();
  void update_glyph(Cloud* cloud);
  void reset_glyph();

private:
  float lidar_height;
  int width;
};

#endif
