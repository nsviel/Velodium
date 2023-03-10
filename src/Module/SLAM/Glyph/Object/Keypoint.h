#ifndef SLAM_OBJECT_KEYPOINT_H
#define SLAM_OBJECT_KEYPOINT_H

#include "../../../../Scene/Glyph/Base/Glyph_source.h"


class Keypoint : public Glyph_source
{
public:
  Keypoint();

public:
  void create_glyph(Cloud* subset);
  void update_keypoint_location(Cloud* subset);
  void update_keypoint_normal(Cloud* subset);

private:
};

#endif
