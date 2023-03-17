#ifndef GPU_FBO_H
#define GPU_FBO_H

#include "../../common.h"

struct FBO{
  std::string name;
  GLuint ID_fbo;
  GLuint ID_tex_color;
  GLuint ID_tex_depth;
  GLuint ID_tex_position;
  GLuint ID_tex_normal;
};


class GPU_fbo
{
public:
  GPU_fbo();
  ~GPU_fbo();

public:
  void init_create_rendering_fbo(int nb_fbo);
  void delete_fbo_all();
  FBO* get_fbo_byName(string querry);

  //GPU_fbo generation stuff
  void gen_fbo(FBO* fbo);
  void gen_fbo_tex_color(FBO* fbo, int attachment_id);
  void gen_fbo_tex_color_multisample(FBO* fbo, int attachment_id);
  void gen_fbo_tex_depth(FBO* fbo);
  void gen_fbo_tex_position(FBO* fbo, int attachment_id);
  void gen_fbo_tex_normal(FBO* fbo, int attachment_id);
  void gen_fbo_check(FBO* fbo);
  void gen_fbo_attachment(FBO* fbo, int nb_attachment);

  //GBuffer stuff
  void create_gbuffer();


  inline vector<FBO*> get_fbo_vec(){return fbo_vec;}

private:
  vector<FBO*> fbo_vec;
  int id_current;
};

#endif
