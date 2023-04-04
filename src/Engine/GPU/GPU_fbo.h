#ifndef GPU_FBO_H
#define GPU_FBO_H

#include "../../common.h"

class Node_engine;
class Dimension;

struct FBO{
  std::string name;
  GLuint ID_fbo;
  GLuint ID_tex_color;
  GLuint ID_tex_position;
  GLuint ID_tex_normal;
  GLuint ID_depth_buffer;
  GLuint ID_buffer_depth;
};
struct Pyramid{
  int nb_lvl;
  GLuint tex_index;
  vector<FBO*> fbo_vec;
  vector<int> size_nn;
};


class GPU_fbo
{
public:
  GPU_fbo(Node_engine* node_engine);
  ~GPU_fbo();

public:
  void init_create_rendering_fbo();
  void delete_fbo_all();
  FBO* create_new_fbo(string name);
  FBO* create_new_pyramid_fbo(string name);
  FBO* get_fbo_byName(string querry);

  //FBO generation
  void gen_fbo(FBO* fbo);
  void gen_fbo_check(FBO* fbo);
  void gen_fbo_attachment(FBO* fbo, int nb_attachment);

  //FBO texture generation
  void gen_fbo_tex_color(FBO* fbo, int attachment_id);
  void gen_fbo_tex_color_multisample(FBO* fbo, int attachment_id);
  void gen_fbo_tex_depth(FBO* fbo);
  void gen_fbo_tex_position(FBO* fbo, int attachment_id);
  void gen_fbo_tex_normal(FBO* fbo, int attachment_id);
  void gen_fbo_tex_pyramide(FBO* fbo, int attachment_id);
  void gen_fbo_tex_sector_idx();

  //GBuffer stuff
  void create_gbuffer();

  inline vector<FBO*> get_fbo_vec(){return fbo_vec;}
  inline Pyramid* get_struct_pyramid(){return struct_pyramid;}

private:
  Pyramid* struct_pyramid;
  Dimension* dimManager;
  vector<FBO*> fbo_vec;
  int id_current;
};

#endif
