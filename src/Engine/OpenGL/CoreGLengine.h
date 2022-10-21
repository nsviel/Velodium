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
class Viewport;
class GUI;
class Renderer;
class Configuration;


class CoreGLengine
{
public:
  CoreGLengine();
  ~CoreGLengine();

public:
  //Argument processing
  void arg(int argc, char* argv[]);

  //Init opengl stuff
  void init();
  void init_OGL();
  void init_object();
  void init_rendering();

  //GL loop
  void loop();
  void loop_gui();
  void loop_selection();
  void loop_pass_1();
  void loop_pass_2();
  void loop_drawScene();
  void loop_drawScreen();
  void loop_end();

  inline GLFWwindow* get_window(){return window;}
  inline Configuration* get_configManager(){return configManager;}
  inline float get_time_loop(){return time_loop;}

private:
  Node_engine* node_engine;
  Node_gui* node_gui;

  GLFWwindow* window;
  Camera* cameraManager;
  Viewport* viewportManager;
  Dimension* dimManager;
  Renderer* renderManager;
  GUI* guiManager;
  Shader* shaderManager;
  Configuration* configManager;

  float time_loop;
  bool openglRunning;
  bool flag_resized;
};

#endif
