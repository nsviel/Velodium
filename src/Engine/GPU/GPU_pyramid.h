#ifndef GPU_PYRAMID_H
#define GPU_PYRAMID_H

#include "GPU_fbo.h"
#include "../../common.h"

class Node_engine;
class GPU_data;
class GPU_fbo;
class Shader;


class GPU_pyramid
{
public:
  GPU_pyramid(Node_engine* node_engine);
  ~GPU_pyramid();

public:
  //Main function
  void bind_pyramid(Object_* canvas);
  void bind_pyramid_lvl_0(Object_* canvas);
  void bind_pyramid_lvl_n(Object_* canvas);
  void bind_pyramid_visibility(Object_* canvas);

  //Subfunction
  void unbind_fboAndTexture(int nb_tex);

private:
  Shader* shaderManager;
  GPU_data* gpuManager;
  GPU_fbo* fboManager;
};

#endif
