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

  glDeleteFramebuffers(1, &fbo_1_ID);
  glDeleteFramebuffers(1, &fbo_2_ID);

  //---------------------------
}

//Init
void GPU_render::init_create_fbo_MSAA(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  glGenTextures(1, &fbo_MSAA_ID);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbo_MSAA_ID);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 2, GL_RGB, gl_dim.x, gl_dim.y, GL_TRUE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fbo_MSAA_ID, 0);

  //---------------------------
}
void GPU_render::init_create_fbo_1(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Create framebuffer 1
  glGenFramebuffers(1, &fbo_1_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);

  //Create (allocate memory) color texture and bind it to the framebuffer
  glGenTextures(1, &fbo_1_tex_color_ID);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_color_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gl_dim.x, gl_dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_1_tex_color_ID, 0);

  //Create depth texture and bind it to the framebuffer
  glGenTextures(1, &fbo_1_tex_depth_ID);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_depth_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo_1_tex_depth_ID, 0);

  //Debind framebuffer
  glBindTexture(GL_TEXTURE_2D ,0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void GPU_render::init_create_fbo_2(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Create framebuffer 2
  glGenTextures(1, &fbo_2_tex_color_ID);
  glGenFramebuffers(1, &fbo_2_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2_ID);

  //Create color texture and bind it to the framebuffer
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbo_2_tex_color_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 64, GL_RGBA, gl_dim.x, gl_dim.y, false);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fbo_2_tex_color_ID, 0);

  //Debind framebuffer
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
  gpuManager->bind_buffer_location(canvas);
  gpuManager->bind_buffer_uvmapping(canvas);
  canvas->draw_type = GL_TRIANGLES;

  //---------------------------
}

//Render
void GPU_render::bind_fbo_screen(){
  //---------------------------

  //Clear framebuffer and enable depth
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //---------------------------
}
void GPU_render::bind_fbo_render(){
  //---------------------------

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2_ID);
  glDisable(GL_DEPTH_TEST);

  //Bind color and depth textures from fbo 1
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_color_ID);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_depth_ID);

  //---------------------------
}
void GPU_render::bind_canvas(){
  //---------------------------

  //Bind fbo and clear old one
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  // Current texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_color_ID);

  //Draw quad
  gpuManager->draw_object(canvas);

  //---------------------------
}

//Update dimensions
void GPU_render::update_dim_texture(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Update texture dimensions
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_color_ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gl_dim.x, gl_dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_depth_ID);
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
