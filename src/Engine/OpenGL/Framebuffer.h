#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../common.h"

class Dimension;


class Framebuffer
{
public:
  Framebuffer(Dimension* dim);
  ~Framebuffer();

public:
  void init_rendering_fbo_1();
  void init_rendering_fbo_2();

private:
  Dimension* dimManager;

  GLuint quad_vao;
  GLuint quad_vbo;
  GLuint fbo_1_ID;
  GLuint fbo_2_ID;
  GLuint texture_ID;
  GLuint tex_color_ID;
  GLuint tex_depth_ID;
  GLuint tex_edl_ID;
};

#endif
