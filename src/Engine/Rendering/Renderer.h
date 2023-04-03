#ifndef RENDERER_H
#define RENDERER_H

#include "../GPU/GPU_fbo.h"
#include "../../common.h"

class Node_engine;
class Dimension;

class Render_pass_1;
class Render_pass_2;
class Render_pass_3;


class Renderer
{
public:
  Renderer(Node_engine* node_engine);
  ~Renderer();

public:
  //Main function
  void loop_rendering();

  //Subfunction
  void update_dim_texture();
  vec4* get_screen_color();

private:
  Dimension* dimManager;
  GPU_fbo* gpu_fbo;

  Render_pass_1* render_pass_1;
  Render_pass_2* render_pass_2;
  Render_pass_3* render_pass_3;
};

#endif
