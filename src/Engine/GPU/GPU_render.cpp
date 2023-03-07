#include "GPU_render.h"
#include "GPU_data.h"

#include "../Core/Dimension.h"
#include "../Core/Configuration.h"

#include <filesystem>
#include <FreeImage.h>
#include <cstdint>

// Framebuffer 1 is for point cloud rendering
// Framebuffer 2 is for EDL rendering


//Constructor / Destructor
GPU_render::GPU_render(Dimension* dim){
  //---------------------------

  this->dimManager = dim;
  this->configManager = new Configuration();
  this->gpuManager = new GPU_data();

  float bkg_color = configManager->parse_json_f("window", "background_color");
  this->screen_color = vec4(bkg_color, bkg_color, bkg_color, 1.0f);

  //---------------------------
}
GPU_render::~GPU_render(){
  //---------------------------

  delete configManager;
  this->delete_fbo_all();

  //---------------------------
}

//Framebuffer generation stuf
void GPU_render::gen_fbo(FBO* fbo){
  //---------------------------

  glGenFramebuffers(1, &fbo->ID_fbo);

  //---------------------------
}
void GPU_render::gen_fbo_tex_color(FBO* fbo){
  vec2 dim = dimManager->get_gl_dim();
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  //Create texture
  glGenTextures(1, &fbo->ID_tex_color);
  glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_color);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.x, dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->ID_tex_color, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void GPU_render::gen_fbo_tex_color_multisample(FBO* fbo){
  vec2 dim = dimManager->get_gl_dim();
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  //Create texture
  glGenTextures(1, &fbo->ID_tex_color);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbo->ID_tex_color);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 64, GL_RGBA, dim.x, dim.y, false);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fbo->ID_tex_color, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void GPU_render::gen_fbo_tex_depth(FBO* fbo){
  vec2 dim = dimManager->get_gl_dim();
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  //Creat texture
  glGenTextures(1, &fbo->ID_tex_depth);
  glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_depth);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dim.x, dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->ID_tex_depth, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}

//Init
void GPU_render::init_create_fbo(){
  nb_fbo = 3;
  //---------------------------

  for(int i=0; i<nb_fbo; i++){
    FBO* fbo = new FBO();
    fbo->name = "fbo_" + to_string(i);
    this->gen_fbo(fbo);
    this->gen_fbo_tex_color(fbo);
    this->gen_fbo_tex_depth(fbo);
    this->fbo_vec.push_back(fbo);
  }

  //---------------------------
}
void GPU_render::init_create_canvas(){
  this->canvas = new Object_();
  //---------------------------

  //Generic quad coordinates and UV
  canvas->xyz.push_back(vec3(-1.0f, 1.0f, 0.0f));
  canvas->xyz.push_back(vec3(-1.0f, -1.0f, 0.0f));
  canvas->xyz.push_back(vec3(1.0f, -1.0f, 0.0f));
  canvas->xyz.push_back(vec3(-1.0f, 1.0f, 0.0f));
  canvas->xyz.push_back(vec3(1.0f, -1.0f, 0.0f));
  canvas->xyz.push_back(vec3(1.0f, 1.0f, 0.0f));

  canvas->uv.push_back(vec2(0.0f,  1.0f));
  canvas->uv.push_back(vec2(0.0f,  0.0f));
  canvas->uv.push_back(vec2(1.0f,  0.0f));
  canvas->uv.push_back(vec2(0.0f,  1.0f));
  canvas->uv.push_back(vec2(1.0f,  0.0f));
  canvas->uv.push_back(vec2(1.0f,  1.0f));

  gpuManager->gen_vao(canvas);
  gpuManager->gen_buffer_location(canvas);
  gpuManager->gen_buffer_uv(canvas);
  canvas->draw_type = GL_TRIANGLES;

  //---------------------------
}

//Render
void GPU_render::bind_fbo_screen(){
  FBO* fbo_1 = fbo_vec[0];
  //---------------------------

  //Binf first fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1->ID_fbo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_1->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_1->ID_tex_depth);

  //Clear framebuffer and enable depth
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //---------------------------
}
void GPU_render::bind_fbo_render(){
  FBO* fbo_1 = fbo_vec[0];
  FBO* fbo_2 = fbo_vec[1];
  //---------------------------

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2->ID_fbo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_1->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_1->ID_tex_depth);

  //Disable depth test
  glDisable(GL_DEPTH_TEST);

  //---------------------------
}
void GPU_render::truc(){
  FBO* fbo_2 = fbo_vec[1];
  FBO* fbo_3 = fbo_vec[2];
  //---------------------------

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_3->ID_fbo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_2->ID_tex_color);

  //Clear old screen
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  //Disable depth test
  glDisable(GL_DEPTH_TEST);

  //---------------------------
}
void GPU_render::bind_canvas(){
  FBO* fbo_1 = fbo_vec[0];
  FBO* fbo_2 = fbo_vec[1];
  FBO* fbo_3 = fbo_vec[2];
  //---------------------------

  //LE PROBLEME CEST QUE IL FAUT REDESSINER LE CANVAS APRES CHAQUE POST PROCESSING effect
  // ESSAYER DE D2M2LER LE CANVAS DE BIND CANVAS !!!!

  //Bind fbo and clear old one
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_1->ID_tex_color);

  //Draw quad
  gpuManager->draw_object(canvas);

  //---------------------------
}

//Update dimensions
void GPU_render::update_dim_texture(){
  vec2 gl_dim = dimManager->get_gl_dim();
  FBO* fbo_1 = fbo_vec[0];
  //---------------------------

  //Update texture dimensions
  glBindTexture(GL_TEXTURE_2D, fbo_1->ID_tex_color);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gl_dim.x, gl_dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(GL_TEXTURE_2D, fbo_1->ID_tex_depth);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

  //---------------------------
}
void GPU_render::update_dim_canvas(){
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
  canvas->xyz.clear();
  canvas->xyz.push_back(tl);
  canvas->xyz.push_back(bl);
  canvas->xyz.push_back(br);

  canvas->xyz.push_back(tl);
  canvas->xyz.push_back(br);
  canvas->xyz.push_back(tr);

  gpuManager->update_buffer_location(canvas);

  //---------------------------
}
void GPU_render::delete_fbo_all(){
  int nb = fbo_vec.size();
  //---------------------------

  for(int i=0; i<nb; i++){
    glDeleteFramebuffers(1, &fbo_vec[i]->ID_fbo);
    delete fbo_vec[i];
  }

  //---------------------------
}
