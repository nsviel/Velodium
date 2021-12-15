#ifndef DEF_SCENEOPENGL
#define DEF_SCENEOPENGL

class Dimension;
class Shader;
class Camera;
class Viewport;
class GUI;
class Engine;
class Framebuffer;
class Configuration;

#include "../../common.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


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

  void trucEDL();

  //GL loop
  void loop();
  void loop_begin();
  void loop_camera(int viewport_ID);
  void loop_shader();
  void loop_end();

private:
  Configuration* configManager;
  GLFWwindow* window;
  Shader* mvpShader;
  Shader* edlShader;
  Shader* fboShader;
  Engine* engineManager;
  Camera* cameraManager;
  Viewport* viewportManager;
  Dimension* dimManager;
  Framebuffer* fboManager;
  GUI* guiManager;

  uint shader_program_ID, mvpID;
  vec3 backgColor;

  int loop_cpt;
  uint quadVAO;
  GLuint fbo_pass_1;
  GLuint fbo_pass_2;
  GLuint texture_color_ID;
  GLuint texture_depth_ID;
  GLuint texture_postProcess_ID;


  GLuint quad_vao, quad_vbo;
  unsigned int FBO;
  unsigned int texture;
};

#endif
