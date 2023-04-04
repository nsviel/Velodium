#ifndef RENDER_PYRAMID_H
#define RENDER_PYRAMID_H

#include "../../GPU/GPU_fbo.h"
#include "../../../common.h"

class Node_engine;
class Shader;
class Camera;
class Dimension;
class GPU_data;
class GPU_fbo;
class GPU_render;


class Render_pyramid
{
public:
  Render_pyramid(Node_engine* node_engine);
  ~Render_pyramid();

public:
  //Main function
  void render_pyramid(Object_* canvas);

  //Subfunction
  void pyramid_lvl_0(Object_* canvas);
  void pyramid_lvl_n(Object_* canvas);
  void pyramid_visibility(Object_* canvas);

private:
  Shader* shaderManager;
  Camera* cameraManager;
  Dimension* dimManager;

  GPU_data* gpu_data;
  GPU_fbo* gpu_fbo;
  GPU_render* gpu_render;
};

#endif
