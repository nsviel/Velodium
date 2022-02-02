#ifndef WIN_CAMERA_H
#define WIN_CAMERA_H

#include "../../common.h"

class Camera;
class config_opengl;


class WIN_camera
{
public:
  //Constructor / Destructor
  WIN_camera(Camera* camera);
  ~WIN_camera();

public:
  //Main function
  void window_camera();

  //Sub functions
  void parameters();
  void information();
  void insert_definedPosition();

private:
  config_opengl* configManager;
  Camera* cameraManager;
};

#endif
