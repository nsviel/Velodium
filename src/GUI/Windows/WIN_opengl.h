#ifndef WIN_OPENGL_H
#define WIN_OPENGL_H

#include "../../common.h"

class Engine_node;
class Camera;
class Configuration;
class Shader;
class PP_edl;


class WIN_opengl
{
public:
  //Constructor / Destructor
  WIN_opengl(Engine_node* node_engine);
  ~WIN_opengl();

public:
  //Main function
  void window_camera();
  void window_shader();

  //Sub functions
  void cam_parameter();
  void cam_info();
  void cam_definedPosition();

private:
  Configuration* configManager;
  Camera* cameraManager;
  Shader* shaderManager;
  PP_edl* edlManager;

  int item_width;
};

#endif
