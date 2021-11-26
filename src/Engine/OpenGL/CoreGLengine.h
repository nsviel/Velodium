#ifndef DEF_SCENEOPENGL
#define DEF_SCENEOPENGL

class Dimension;
class Shader;
class Camera;
class GUI;
class Engine;
class Framebuffer;
class Configuration;

#include "../../common.h"

class CoreGLengine
{
public:
  CoreGLengine();
  ~CoreGLengine();

public:
  bool init();
  bool init_OGL();
  bool init_object();
  bool init_shader();

  //GL loop
  void loop();
  void loop_begin();
  void loop_camera();
  void loop_shader();
  void loop_end();

private:
  Configuration* configManager;
  GLFWwindow* window;
  Shader* mvp_shaderManager;
  Engine* engineManager;
  Camera* cameraManager;
  Dimension* dimManager;
  Framebuffer* fboManager;
  GUI* guiManager;

  uint shader_program_ID, mvpID;
  vec3 backgColor;
};

#endif
