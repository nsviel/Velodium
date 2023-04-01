#ifndef GPU_RENDER_H
#define GPU_RENDER_H

#include "../../common.h"

class GPU_data;


class GPU_render
{
public:
  GPU_render();
  ~GPU_render();

public:
  void unbind_fboAndTexture(int nb_tex);
  Object_* gen_canvas();

private:
  GPU_data* gpu_data;
};

#endif
