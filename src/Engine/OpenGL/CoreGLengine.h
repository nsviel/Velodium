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

class Dimension;
class Shader;
class Camera;
class Viewport;
class GUI;
class Engine;
class Renderer;
class Engine_node;
class Configuration;
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
  void loop_pass_1();
  void loop_pass_2();
  void loop_drawScene();
  void loop_drawScreen();
  void loop_end();

private:
  Engine_node* node_engine;

  GLFWwindow* window;
  Engine* engineManager;
  Camera* cameraManager;
  Viewport* viewportManager;
  Dimension* dimManager;
  Renderer* renderManager;
  GUI* guiManager;
  Shader* shaderManager;
  Configuration* configManager;

  bool is_visualization;
  bool openglRunning;
  bool flag_resized;
};

#endif
