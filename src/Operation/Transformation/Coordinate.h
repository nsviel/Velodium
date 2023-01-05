#ifndef COORDINATE_H
#define COORDINATE_H

#include "../../common.h"

class Camera;
class Dimension;
class Node_operation;


class Coordinate
{
public:
  //Constructor / Destructor
  Coordinate(Node_operation* node);
  ~Coordinate();

public:
  vec3 ScreenToWorld(vec2 cursorPos);
  vec2 WorldToScreen(vec3 point);
  vec3 CursorToGround();

private:
  Camera* cameraManager;
  Dimension* dimManager;
};

#endif
