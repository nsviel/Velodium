#ifndef SLAM_OBJECT_KEYPOINT_H
#define SLAM_OBJECT_KEYPOINT_H

#include "../../../../Scene/Glyph/Base/Glyph_source.h"


class Keypoint : public Glyph_source
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
