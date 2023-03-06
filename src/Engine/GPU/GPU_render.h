#ifndef GPU_RENDER_H
#define GPU_RENDER_H

#include "../../common.h"

class Dimension;
class Configuration;
class GPU_data;


class GPU_render
{
public:
  GPU_render(Dimension* dim);
  ~GPU_render();

public:
  void init_create_fbo_MSAA();
  void init_create_fbo_1();
  void init_create_fbo_2();
  void init_create_canvas();

  void bind_fbo_screen();
  void bind_fbo_render();
  void bind_canvas();

  void update_dim_texture();
  void update_dim_canvas();

  inline vec4* get_screen_color(){return &screen_color;}

private:
  Dimension* dimManager;
  Configuration* configManager;
  GPU_data* gpuManager;

  Object_* canvas;

  GLuint fbo_MSAA_ID;
  GLuint fbo_1_ID;
  GLuint fbo_1_tex_color_ID;
  GLuint fbo_1_tex_depth_ID;
  GLuint fbo_2_ID;
  GLuint fbo_2_tex_color_ID;

  vec4 screen_color;
};

#endif
