#include "GPU_rendering.h"
#include "GPU_data.h"

#include "../Node_engine.h"
#include "../Core/Dimension.h"
#include "../Core/Configuration.h"
#include "../Shader/Shader.h"

#include <filesystem>
#include <FreeImage.h>
#include <cstdint>


//Constructor / Destructor
GPU_rendering::GPU_rendering(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();
  this->shaderManager = node_engine->get_shaderManager();
  this->configManager = new Configuration();
  this->gpuManager = new GPU_data();
  this->fboManager = new Framebuffer();

  float bkg_color = configManager->parse_json_f("window", "background_color");
  this->screen_color = vec4(bkg_color, bkg_color, bkg_color, 1.0f);

  //---------------------------
}
GPU_rendering::~GPU_rendering(){
  //---------------------------

  fboManager->delete_fbo_all();

  delete configManager;
  delete fboManager;
  delete gpuManager;

  //---------------------------
}

//Loop function
void GPU_rendering::loop_pass_1(){
  //---------------------------

  //Bind first fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_vec[0]->ID_fbo);

  //Clear framebuffer and enable depth
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //---------------------------
}
void GPU_rendering::loop_pass_2(){
  //---------------------------

  //ESSAYER DINVERSER LE SENS DES SHADER EDL ET LE SHADER COLOR INV

  //EDL shader
  shaderManager->use_shader("render_edl");
  this->bind_fbo_pass_2_edl();

  //Color inversion
  shaderManager->use_shader("render_color_inv");
  this->bind_fbo_pass_2_inv();

  //Draw screen quad
  shaderManager->use_shader("canvas");
  this->bind_canvas();

  //---------------------------
}

//Rendering
void GPU_rendering::bind_fbo_pass_2_edl(){
  FBO* fbo_1 = fbo_vec[0];
  FBO* fbo_2 = fbo_vec[1];
  vec2 dim = dimManager->get_win_dim();
  //---------------------------

  //Disable depth test
  glDisable(GL_DEPTH_TEST);

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2->ID_fbo);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_1->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_vec[0]->ID_tex_depth);

  gpuManager->draw_object(canvas_render);

  //Unbind
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void GPU_rendering::bind_fbo_pass_2_inv(){
  FBO* fbo_1 = fbo_vec[0];
  FBO* fbo_2 = fbo_vec[1];
  FBO* fbo_3 = fbo_vec[2];
  //---------------------------

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_3->ID_fbo);

  //input
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_2->ID_tex_color);

  gpuManager->draw_object(canvas_render);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void GPU_rendering::bind_canvas(){
  FBO* fbo_1 = fbo_vec[0];
  FBO* fbo_2 = fbo_vec[1];
  FBO* fbo_3 = fbo_vec[2];
  //---------------------------

  //Bind fbo and clear old one
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_3->ID_tex_color);

  //Draw quad
  gpuManager->draw_object(canvas_screen);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  //---------------------------
}

//Init function
void GPU_rendering::init_create_fbo(int nb_shader){
  //---------------------------

  for(int i=0; i<nb_shader; i++){
    FBO* fbo = new FBO();
    fboManager->gen_fbo(fbo);
    fboManager->gen_fbo_tex_color(fbo);
    fboManager->gen_fbo_tex_depth(fbo);
    fboManager->gen_fbo_check(fbo);
    this->fbo_vec.push_back(fbo);
  }

  //---------------------------
}
void GPU_rendering::init_create_canvas(){
  //---------------------------

  this->canvas_screen = gen_canvas();
  this->canvas_render = gen_canvas();

  //---------------------------
}

//Update
void GPU_rendering::update_dim_texture(){
  //---------------------------

  vec2 dim = dimManager->get_win_dim();
  for(int i=0; i<fbo_vec.size(); i++){
    FBO* fbo = fbo_vec[i];
    glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim.x, dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dim.x, dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  }


  //---------------------------
}
void GPU_rendering::update_dim_canvas(){
  //---------------------------

  //Compute canvas coordinates
  vec2 gl_pos = dimManager->get_gl_pos();
  vec2 gl_dim = dimManager->get_gl_dim();
  vec2 win_dim = dimManager->get_win_dim();

  vec3 tl, br, tr, bl;
  bl.x = 2 * (gl_pos.x) / (win_dim.x) - 1;
  bl.y = 2 * (gl_pos.y) / (win_dim.y) - 1;
  bl.z = 0.0f;

  br.x = 1;
  br.y = 2 * (gl_pos.y) / (win_dim.y) - 1;
  br.z = 0.0f;

  tl.x = 2 * (gl_pos.x) / (win_dim.x) - 1;
  tl.y = 2 * (gl_pos.y + gl_dim.y) / (win_dim.y) - 1;
  tl.z = 0.0f;

  tr.x = 1;
  tr.y = 2 * (gl_pos.y + gl_dim.y) / (win_dim.y) - 1;
  tr.z = 0.0f;

  //Update canvas location buffer
  canvas_screen->xyz.clear();
  canvas_screen->xyz.push_back(tl);
  canvas_screen->xyz.push_back(bl);
  canvas_screen->xyz.push_back(br);

  canvas_screen->xyz.push_back(tl);
  canvas_screen->xyz.push_back(br);
  canvas_screen->xyz.push_back(tr);

  gpuManager->update_buffer_location(canvas_screen);

  //---------------------------
}
Object_* GPU_rendering::gen_canvas(){
  Object_* canvas = new Object_();
  //---------------------------

  //Generic quad coordinates and UV
  vector<vec3> xyz;
  xyz.push_back(vec3(-1.0f, 1.0f, 0.0f));
  xyz.push_back(vec3(-1.0f, -1.0f, 0.0f));
  xyz.push_back(vec3(1.0f, -1.0f, 0.0f));
  xyz.push_back(vec3(-1.0f, 1.0f, 0.0f));
  xyz.push_back(vec3(1.0f, -1.0f, 0.0f));
  xyz.push_back(vec3(1.0f, 1.0f, 0.0f));

  vector<vec2> uv;
  uv.push_back(vec2(0.0f,  1.0f));
  uv.push_back(vec2(0.0f,  0.0f));
  uv.push_back(vec2(1.0f,  0.0f));
  uv.push_back(vec2(0.0f,  1.0f));
  uv.push_back(vec2(1.0f,  0.0f));
  uv.push_back(vec2(1.0f,  1.0f));

  canvas->xyz = xyz;
  canvas->uv = uv;

  gpuManager->gen_vao(canvas);
  gpuManager->gen_buffer_location(canvas);
  gpuManager->gen_buffer_uv(canvas);
  canvas->draw_type = GL_TRIANGLES;

  //---------------------------
  return canvas;
}
