#ifndef OBJECT_KEYPOINT_H
#define OBJECT_KEYPOINT_H

#include "../../Base/Object_base.h"


class Keypoint : public Object_base
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
