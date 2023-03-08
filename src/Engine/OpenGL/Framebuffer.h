#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../common.h"

struct FBO{
  GLuint ID_fbo;
  GLuint ID_tex_color;
  GLuint ID_tex_depth;
};


class Framebuffer
{
public:
  Framebuffer();
  ~Framebuffer();

public:
  void delete_fbo_all();
  
  //Framebuffer generation stuff
  void gen_fbo(FBO* fbo);
  void gen_fbo_tex_color(FBO* fbo);
  void gen_fbo_tex_color_multisample(FBO* fbo);
  void gen_fbo_tex_depth(FBO* fbo);
  void gen_fbo_check(FBO* fbo);

private:
  vector<FBO*> fbo_vec;
  int id_current;
};

#endif
