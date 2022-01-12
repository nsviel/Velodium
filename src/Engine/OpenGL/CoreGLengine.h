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
#include <glm/gtc/matrix_transform.hpp>


class CoreGLengine
{
public:
  CoreGLengine();
  ~CoreGLengine();

public:
  bool init();
  bool init_config();
  bool init_OGL();
  bool init_object();
  bool init_shader();
  void init_quad();
  void init_fbo();

  //GL loop
  void loop();
  void loop_pass_1();
  void loop_pass_2();
  void loop_drawScene();
  void loop_drawQuad();
  void loop_end();

  void update_gl_quad();

private:
  Configuration* configManager;
  GLFWwindow* window;
  Engine* engineManager;
  Camera* cameraManager;
  Viewport* viewportManager;
  Dimension* dimManager;
  Framebuffer* fboManager;
  GUI* guiManager;
  Shader* shaderManager;

  vec3 backgColor;
  int gl_width;
  int gl_height;

  GLuint quad_vao;
  GLuint quad_vbo;
  GLuint fbo_1_ID;
  GLuint fbo_2_ID;
  GLuint texture_ID;
  GLuint tex_color_ID;
  GLuint tex_depth_ID;
  GLuint tex_edl_ID;
};

#endif
