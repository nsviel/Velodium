#ifndef GPU_RENDER_H
#define GPU_RENDER_H

#include "../OpenGL/Framebuffer.h"
#include "../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class GPU_data;
class Shader;


class GPU_rendering
{
public:
  GPU_rendering(Node_engine* node_engine);
  ~GPU_rendering();

public:
  //Loop function
  void loop_pass_1();
  void loop_pass_2();

  //Init function
  void init_create_fbo(int nb_shader);
  void init_create_canvas();

  //Rendering
  void bind_fbo_pass_2_edl();
  void bind_fbo_pass_2_inv();
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
  Framebuffer* fboManager;

  Object_* canvas_render;
  Object_* canvas_screen;

  vector<FBO*> fbo_vec;
  vec4 screen_color;
};

#endif
