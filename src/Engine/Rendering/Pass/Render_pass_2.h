#ifndef RENDER_PASS_2_H
#define RENDER_PASS_2_H

#include "../../GPU/GPU_fbo.h"
#include "../../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class GPU_data;
class GPU_render;
class Shader;
class Camera;
class Render_pyramid;


class Render_pass_2
{
public:
  Render_pass_2(Node_engine* node_engine);
  ~Render_pass_2();

public:
  //Main function
  void compute_pass();

  //Subfunction
  void configure_opengl();
  void render_recombination();
  void render_edl();

  //vec3 fct_unproject(vec2 coord_frag);

private:
  Dimension* dimManager;
  Shader* shaderManager;
  Camera* cameraManager;
  Render_pyramid* pyramidManager;

  GPU_data* gpu_data;
  GPU_fbo* gpu_fbo;
  GPU_render* gpu_render;

  Object_* canvas;
};

#endif
