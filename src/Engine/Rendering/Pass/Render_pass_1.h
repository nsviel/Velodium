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
  void compute_pass();
  void configure_opengl();

  //Render mesh function
  void render_mesh();
  void render_mesh_fbo();
  void render_mesh_light();
  void render_mesh_untextured();
  void render_mesh_textured();

  //Specific cloud-to-render function
  void render_mrt();
  void render_mrt_gfbo();
  void render_mrt_shader();

  inline vec4* get_screen_color(){return &screen_color;}

private:
  Dimension* dimManager;
  Shader* shaderManager;
  Camera* cameraManager;
  Engine* engineManager;
  Configuration* configManager;
  GPU_fbo* gpu_fbo;

  vec4 screen_color;
};

#endif
