#ifndef CoordTransform_H
#define CoordTransform_H

class Camera;
class Dimension;

#include "../../common.h"

class CoordTransform
{
public:
  //Constructor / Destructor
  CoordTransform(Camera* controls, Dimension* dim);
  ~CoordTransform();

public:
  vec3 ScreenToWorld(vec2 cursorPos);
  vec2 WorldToScreen(vec3 point);
  vec3 CursorToGround();

private:
  Camera* cameraManager;
  Dimension* dimManager;
};

#endif
