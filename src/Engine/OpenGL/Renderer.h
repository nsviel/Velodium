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
  void init_create_fbo_1();
  void init_create_fbo_2();
  void init_create_canvas();

  void bind_fbo_screen();
  void bind_fbo_render();
  void bind_canvas();

  void update_dim_texture();
  void update_dim_canvas();

  void render_screenshot(string path);
  void render_screenshot_stb_image(string path);
  void render_screenshot_pbo(string path);
  void render_screenshot_freeimage(string path);
  void render_screenshot_online();
  
  inline vec4* get_screen_color(){return &screen_color;}
  inline string* get_save_path(){return &save_path_screenshot;}
  inline bool* get_is_screenshot(){return &is_screenshot;}
  inline float get_time_screenshot(){return time_screenshot;}

private:
  Dimension* dimManager;
  Configuration* configManager;

  GLuint canvas_vao;
  GLuint canvas_vbo_xy;
  GLuint fbo_1_ID;
  GLuint fbo_1_tex_color_ID;
  GLuint fbo_1_tex_depth_ID;
  GLuint fbo_2_ID;
  GLuint fbo_2_tex_color_ID;
  GLuint pbo_screenshot;

  vec4 screen_color;
  float time_screenshot;
  string save_path_screenshot;
  bool is_screenshot;
};

#endif
