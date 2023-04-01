#ifndef COREGLENGINE_H
#define COREGLENGINE_H

#include "../../common.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Namespaces
using namespace std;
using namespace glm;
using namespace Eigen;

class Node_engine;
class Node_gui;

class Dimension;
class Shader;
class Camera;
class GUI;
class Viewport;
class Renderer;
class GPU_screenshot;
class Configuration;
class Engine;


class CoreGLengine
{
public:
  CoreGLengine();
  ~CoreGLengine();

public:
  //Opengl stuff
  void arg(int argc, char* argv[]);
  void init();
  void loop();

  // Initialization stuff
  void init_opengl();
  void init_object();
  void init_rendering();

  // Loop stuff
  void loop_gui();
  void loop_resizing();
  void loop_end();

  inline GLFWwindow* get_window(){return window;}
  inline Configuration* get_configManager(){return configManager;}
  inline float get_time_loop(){return time_loop;}

private:
  Node_engine* node_engine;
  Node_gui* node_gui;

  GLFWwindow* window;
  Camera* cameraManager;
  Dimension* dimManager;
  Renderer* renderManager;
  GPU_screenshot* screenshotManager;
  GUI* guiManager;
  Shader* shaderManager;
  Viewport* viewportManager;
  Configuration* configManager;
  Engine* engineManager;

  float time_loop;
  bool loop_run;
  bool flag_resized;
};

#endif
