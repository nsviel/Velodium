#ifndef CoordTransform_H
#define CoordTransform_H

#include "../../common.h"

class Camera;
class Dimension;
class Node_operation;


class CoordTransform
{
public:
  //Constructor / Destructor
  CoordTransform(Node_operation* node);
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
