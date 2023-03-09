#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../common.h"

struct FBO{
  std::string name;
  GLuint ID_fbo;
  GLuint ID_tex_color;
  GLuint ID_tex_depth;
  GLuint ID_tex_position;
  GLuint ID_tex_normal;
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
  void gen_fbo_tex_color(FBO* fbo, int color_id);
  void gen_fbo_tex_color_multisample(FBO* fbo, int color_id);
  void gen_fbo_tex_depth(FBO* fbo);
  void gen_fbo_tex_position(FBO* fbo, int color_id);
  void gen_fbo_tex_normal(FBO* fbo, int color_id);
  void gen_fbo_check(FBO* fbo);

  //GBuffer stuff
  void create_gbuffer();
  void draw_buffer(FBO* fbo, int nb_attachment);

  inline vector<FBO*> get_fbo_vec(){return fbo_vec;}

private:
  vector<FBO*> fbo_vec;
  int id_current;
};

#endif
