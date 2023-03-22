#ifndef GPU_RENDER_H
#define GPU_RENDER_H

#include "GPU_fbo.h"
#include "../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class GPU_data;
class Shader;
class Camera;
class Engine;

class GPU_rendering
{
public:
  GPU_rendering(Node_engine* node_engine);
  ~GPU_rendering();

public:
  //Loop function
  void init_renderer();
  void loop_pass_1();
  void loop_pass_2();

  //Rendering
  void bind_fbo_pass_2_recombination();
  void bind_fbo_pass_2_edl();
  void bind_fbo_pass_2_pyramid();
  void bind_canvas();

  //Update
  void update_dim_texture();
  void update_dim_canvas();
  Object_* gen_canvas();

  inline vec4* get_screen_color(){return &screen_color;}

private:
  Dimension* dimManager;
  Configuration* configManager;
  GPU_data* gpuManager;
  Shader* shaderManager;
  GPU_fbo* fboManager;
  Engine* engineManager;
  Camera* cameraManager;

  Object_* canvas_render;
  Object_* canvas_screen;
  vec4 screen_color;
};

#endif
