#ifndef SLAM_OBJECT_KEYPOINT_H
#define SLAM_OBJECT_KEYPOINT_H

#include "../Base/SLAM_object_base.h"


class Keypoint : public SLAM_object_base
{
public:
  Keypoint();

public:
  void create_glyph(Subset* subset);
  void update_keypoint_location(Subset* subset);
  void update_keypoint_normal(Subset* subset);

private:
};

#endif
