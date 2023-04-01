#ifndef RENDER_PASS_1_H
#define RENDER_PASS_1_H

#include "../../GPU/GPU_fbo.h"
#include "../../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class Shader;
class Camera;
class Engine;


class Render_pass_1
{
public:
  Render_pass_1(Node_engine* node_engine);
  ~Render_pass_1();

public:
  //Main function
  void loop_pass_1();

  //Subfunction
  void configure_opengl();
  void render_simple_mesh();
  void render_gfbo();

private:
  Dimension* dimManager;
  Shader* shaderManager;
  Camera* cameraManager;
  Engine* engineManager;
  Configuration* configManager;
  GPU_fbo* fboManager;

  vec4 screen_color;
};

#endif
