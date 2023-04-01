#ifndef RENDERER_H
#define RENDERER_H

#include "../GPU/GPU_fbo.h"
#include "../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class GPU_data;
class Shader;
class Camera;
class Engine;
class Render_pyramid;

class Render_pass_1;
class Render_pass_2;
class Render_pass_3;


class Renderer
{
public:
  Renderer(Node_engine* node_engine);
  ~Renderer();

public:
  //Loop function
  void init_renderer();
  void loop_rendering();


  vec3 fct_unproject(vec2 coord_frag);

  //Rendering
  void bind_fbo_pass_2_recombination();
  void bind_fbo_pass_2_edl();
  void bind_canvas();

  //Update
  void update_dim_texture();
  void update_dim_canvas();

  //Subfunction
  Object_* gen_canvas();
  void unbind_fboAndTexture(int nb_tex);

  inline vec4* get_screen_color(){return &screen_color;}

private:
  Dimension* dimManager;
  Configuration* configManager;
  GPU_data* gpu_data;
  Shader* shaderManager;
  GPU_fbo* gpu_fbo;
  Engine* engineManager;
  Camera* cameraManager;
  Render_pyramid* pyramidManager;

  Render_pass_1* render_pass_1;
  Render_pass_2* render_pass_2;
  Render_pass_3* render_pass_3;

  Object_* canvas_render;
  Object_* canvas_screen;
  vec4 screen_color;
};

#endif
