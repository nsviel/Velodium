#ifndef RENDERER_H
#define RENDERER_H

#include "../../../common.h"

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
  void render_screenshot_stb_image(string path);
  void render_screenshot_pbo(string path);
  void render_screenshot_freeimage(string path);
  void render_screenshot_online();

  void update_texture();
  void update_quad();

  inline vec4* get_screen_color(){return &screen_color;}
  inline string* get_save_path(){return &save_path;}
  inline bool* get_is_screenshot(){return &is_screenshot;}
  inline float get_time_screenshot(){return time_screenshot;}

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
  GLuint pbo;

  vec4 screen_color;
  float time_screenshot;
  string save_path;
  bool is_screenshot;
  bool with_fullscreen;
};

#endif
