#ifndef RENDER_PASS_3_H
#define RENDER_PASS_3_H

#include "../../GPU/GPU_fbo.h"
#include "../../../common.h"

class Node_engine;
class Dimension;
class GPU_fbo;
class Shader;
class GPU_data;
class GPU_render;


class Render_pass_3
{
public:
  Render_pass_3(Node_engine* node_engine);
  ~Render_pass_3();

public:
  //Main function
  void compute_pass();

  //Subfunction
  void bind_canvas();

private:
  Dimension* dimManager;
  Shader* shaderManager;
  GPU_fbo* fboManager;
  GPU_data* gpu_data;
  GPU_render* gpu_render;

  Object_* canvas;
};

#endif
