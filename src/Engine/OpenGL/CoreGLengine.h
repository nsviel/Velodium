#ifndef COREGLENGINE_H
#define COREGLENGINE_H

class Dimension;
class Shader;
class Camera;
class Viewport;
class GUI;
class Engine;
class Renderer;
class Configuration;

#include "../../common.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CoreGLengine
{
public:
  CoreGLengine();
  ~CoreGLengine();

public:
  bool init();
  bool init_OGL();
  bool init_object();
  void init_rendering();

  //GL loop
  void loop();
  void loop_gui();
  void loop_pass_1();
  void loop_pass_2();
  void loop_drawScene();
  void loop_drawScreen();
  void loop_end();

private:
  Configuration* configManager;
  GLFWwindow* window;
  Engine* engineManager;
  Camera* cameraManager;
  Viewport* viewportManager;
  Dimension* dimManager;
  Renderer* renderManager;
  GUI* guiManager;
  Shader* shaderManager;

  bool flag_resized;
};

#endif
