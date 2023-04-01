#ifndef RENDER_PASS_1_H
#define RENDER_PASS_1_H

#include "../GPU/GPU_fbo.h"
#include "../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class GPU_data;
class Shader;
class Camera;
class Engine;
class render_pyramid;


class render_pass_1
{
public:
  render_pass_1(Node_engine* node_engine);
  ~render_pass_1();

public:
  //Loop function
  void init_renderer();
  void loop_pass_1();
  void loop_pass_2();


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
  GPU_data* gpuManager;
  Shader* shaderManager;
  GPU_fbo* fboManager;
  Engine* engineManager;
  Camera* cameraManager;
  render_pyramid* pyramidManager;

  Object_* canvas_render;
  Object_* canvas_screen;
  vec4 screen_color;
};

#endif
