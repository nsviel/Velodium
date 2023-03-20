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
  this->fboManager = new GPU_fbo();

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
void GPU_rendering::init_renderer(){
  //---------------------------

  fboManager->init_create_rendering_fbo();
  this->canvas_screen = gen_canvas();
  this->canvas_render = gen_canvas();

  //---------------------------
}
void GPU_rendering::loop_pass_1(){
  vector<FBO*> fbo_vec = fboManager->get_fbo_vec();
  FBO* gfbo = fboManager->get_fbo_byName("gfbo");
  FBO* fbo_1 = fbo_vec[0];
  //---------------------------

  //Bind first fbo
  glBindFramebuffer(GL_FRAMEBUFFER, gfbo->ID_fbo);

  //Clear framebuffer and enable depth
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //---------------------------
}
void GPU_rendering::loop_pass_2(){
  //---------------------------

  //Occlusion
  shaderManager->use_shader("pyramid");
  this->bind_fbo_pass_2_occlusion();

  //EDL shader
  shaderManager->use_shader("render_edl");
  this->bind_fbo_pass_2_edl();

  //Draw screen quad
  shaderManager->use_shader("canvas");
  this->bind_canvas();

  //---------------------------
}

//Rendering
void GPU_rendering::bind_fbo_pass_2_occlusion(){
  FBO* gfbo = fboManager->get_fbo_byName("gfbo");
  FBO* fbo_occ = fboManager->get_fbo_byName("pyramid");
  //---------------------------

  //Disable depth test
  glDisable(GL_DEPTH_TEST);

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_occ->ID_fbo);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_tex_depth);

  gpuManager->draw_object(canvas_render);

  //Unbind
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void GPU_rendering::bind_fbo_pass_2_edl(){
  FBO* gfbo = fboManager->get_fbo_byName("gfbo");
  FBO* fbo_edl = fboManager->get_fbo_byName("edl");
  FBO* fbo_occ = fboManager->get_fbo_byName("pyramid");
  vec2 dim = dimManager->get_win_dim();
  //---------------------------

  //Disable depth test
  glDisable(GL_DEPTH_TEST);

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_edl->ID_fbo);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_occ->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_tex_depth);

  gpuManager->draw_object(canvas_render);

  //Unbind
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void GPU_rendering::bind_canvas(){
  FBO* gfbo = fboManager->get_fbo_byName("gfbo");
  FBO* fbo_edl = fboManager->get_fbo_byName("edl");
  //---------------------------

  //Bind fbo and clear old one
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_edl->ID_tex_color);

  //Draw quad
  gpuManager->draw_object(canvas_screen);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  //---------------------------
}

//Update
void GPU_rendering::update_dim_texture(){
  vector<FBO*> fbo_vec = fboManager->get_fbo_vec();
  //---------------------------

  vec2 dim = dimManager->get_win_dim();
  for(int i=0; i<fbo_vec.size(); i++){
    FBO* fbo = fbo_vec[i];
    glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.x, dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dim.x, dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, dim.x, dim.y, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, dim.x, dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
  }

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);

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
