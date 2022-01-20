#ifndef RENDERER_H
#define RENDERER_H

#include "../../common.h"

class Dimension;
class Configuration;


class Renderer
{
public:
  Renderer(Dimension* dim);
  ~Renderer();

public:
  void init_rendering_fbo_1();
  void init_rendering_fbo_2();
  void init_rendering_quad();

  void render_fbo_1();
  void render_fbo_2();
  void render_quad();
  void render_screenshot(string path);

  void update_texture();
  void update_quad();

  inline vec4* get_screen_color(){return &screen_color;}

private:
  Dimension* dimManager;
  Configuration* configManager;

  GLuint quad_vao;
  GLuint quad_vbo;
  GLuint fbo_1_ID;
  GLuint fbo_2_ID;
  GLuint tex_color_ID;
  GLuint tex_depth_ID;
  GLuint tex_edl_ID;

  vec4 screen_color;
  bool with_fullscreen;
};

#endif
