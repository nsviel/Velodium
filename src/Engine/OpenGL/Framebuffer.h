#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../common.h"

struct FBO{
  std::string name;
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
  void init_create_fbo(int nb_fbo);
  void delete_fbo_all();
  FBO* get_fbo_byName(string querry);

  //Framebuffer generation stuff
  void gen_fbo(FBO* fbo);
  void gen_fbo_tex_color(FBO* fbo);
  void gen_fbo_tex_color_multisample(FBO* fbo);
  void gen_fbo_tex_depth(FBO* fbo);
  void gen_fbo_check(FBO* fbo);

  inline vector<FBO*> get_fbo_vec(){return fbo_vec;}

private:
  vector<FBO*> fbo_vec;
  int id_current;
};

#endif
