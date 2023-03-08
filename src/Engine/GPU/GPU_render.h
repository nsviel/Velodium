#ifndef GPU_RENDER_H
#define GPU_RENDER_H

#include "../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class GPU_data;
class Shader;

struct FBO{
  GLuint ID_fbo;
  GLuint ID_tex_color;
  GLuint ID_tex_depth;
};

struct FBOManager{
  vector<FBO*> fbo_vec;
  int id_current;
};


class GPU_render
{
public:
  GPU_render(Node_engine* node_engine);
  ~GPU_render();

public:
  //Loop function
  void loop_pass_1();
  void loop_pass_2();
  
  //Init function
  void init_create_fbo(int nb_shader);
  void init_create_canvas();

  //Framebuffer generation stuff
  void gen_fbo(FBO* fbo);
  void gen_fbo_tex_color(FBO* fbo);
  void gen_fbo_tex_color_multisample(FBO* fbo);
  void gen_fbo_tex_depth(FBO* fbo);
  void gen_fbo_check(FBO* fbo);
  Object_* gen_canvas();

  //Rendering
  void bind_fbo_pass_2_edl();
  void bind_fbo_pass_2_inv();
  void bind_canvas();

  //Update
  void update_dim_texture();
  void update_dim_canvas();
  void delete_fbo_all();

  inline vec4* get_screen_color(){return &screen_color;}

private:
  Dimension* dimManager;
  Configuration* configManager;
  GPU_data* gpuManager;
  Shader* shaderManager;

  Object_* canvas_render;
  Object_* canvas_screen;

  vector<FBO*> fbo_vec;
  vec4 screen_color;
};

#endif
