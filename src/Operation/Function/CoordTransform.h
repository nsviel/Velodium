#ifndef CoordTransform_H
#define CoordTransform_H

#include "../../common.h"

class Camera;
class Dimension;
class Operation_node;


class CoordTransform
{
public:
  //Constructor / Destructor
  CoordTransform(Operation_node* node);
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
