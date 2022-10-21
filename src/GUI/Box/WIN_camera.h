#ifndef WIN_CAMERA_H
#define WIN_CAMERA_H

#include "../../common.h"

class Node_engine;
class Camera;
class Configuration;


class WIN_camera
{
public:
  //Constructor / Destructor
  WIN_camera(Node_engine* node_engine);
  ~WIN_camera();

public:
  //Main function
  void window_camera();

  //Sub functions
  void cam_parameter();
  void cam_info();
  void cam_definedPosition();

private:
  Configuration* configManager;
  Camera* cameraManager;

  int item_width;
};

#endif
