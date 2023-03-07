#ifndef GPU_RENDER_H
#define GPU_RENDER_H

#include "../../common.h"

class Dimension;
class Configuration;
class GPU_data;

struct FBO{
  string name;
  GLuint ID_fbo;
  GLuint ID_tex_color;
  GLuint ID_tex_depth;
};


class GPU_render
{
public:
  GPU_render(Dimension* dim);
  ~GPU_render();

public:
  //Framebuffer generation stuff
  void gen_fbo(FBO* fbo);
  void gen_fbo_tex_color(FBO* fbo);
  void gen_fbo_tex_color_multisample(FBO* fbo);
  void gen_fbo_tex_depth(FBO* fbo);

  void init_create_fbo();
  void init_create_canvas();

  void bind_fbo_screen();
  void bind_fbo_render();
  void bind_canvas();

  void update_dim_texture();
  void update_dim_canvas();
  void delete_fbo_all();

  void truc();

  inline vec4* get_screen_color(){return &screen_color;}

private:
  Dimension* dimManager;
  Configuration* configManager;
  GPU_data* gpuManager;

  vector<FBO*> fbo_vec;
  Object_* canvas;
  vec4 screen_color;
  int nb_fbo;
};

#endif
