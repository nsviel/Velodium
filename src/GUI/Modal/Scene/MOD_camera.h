#ifndef MOD_CAMERA_H
#define MOD_CAMERA_H

#include "../../../common.h"

class Node_engine;
class Camera;
class Configuration;


class MOD_camera
{
public:
  //Constructor / Destructor
  MOD_camera(Node_engine* node_engine);
  ~MOD_camera();

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
