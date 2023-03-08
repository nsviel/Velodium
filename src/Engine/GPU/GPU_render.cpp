#include "GPU_render.h"
#include "GPU_data.h"

#include "../Node_engine.h"
#include "../Core/Dimension.h"
#include "../Core/Configuration.h"
#include "../Shader/Shader.h"

#include <filesystem>
#include <FreeImage.h>
#include <cstdint>


//Constructor / Destructor
GPU_render::GPU_render(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();
  this->shaderManager = node_engine->get_shaderManager();
  this->configManager = new Configuration();
  this->gpuManager = new GPU_data();

  float bkg_color = configManager->parse_json_f("window", "background_color");
  this->screen_color = vec4(bkg_color, bkg_color, bkg_color, 1.0f);

  //---------------------------
}
GPU_render::~GPU_render(){
  //---------------------------

  delete configManager;
  delete dimManager;
  delete gpuManager;
  this->delete_fbo_all();

  //---------------------------
}

//Loop function
void GPU_render::loop_pass_1(){
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
void GPU_render::loop_pass_2(){
  //---------------------------

  //Framebuffer pass 2
  //Set EDL shader
  shaderManager->use_shader("render_edl");
  this->bind_fbo_pass_2_edl();


  shaderManager->use_shader("render_color_inv");
  this->bind_fbo_pass_2_inv();

  //Draw screen quad
  shaderManager->use_shader("canvas");
  this->bind_canvas();

  //---------------------------
}

//Init function
void GPU_render::init_create_fbo(int nb_shader){
  //---------------------------

  for(int i=0; i<nb_shader; i++){
    FBO* fbo = new FBO();
    this->gen_fbo(fbo);
    this->gen_fbo_tex_color(fbo);
    this->gen_fbo_tex_depth(fbo);
    this->gen_fbo_check(fbo);
    this->fbo_vec.push_back(fbo);
  }

  //---------------------------
}
void GPU_render::init_create_canvas(){
  //---------------------------

  this->canvas_screen = gen_canvas();
  this->canvas_render = gen_canvas();

  //---------------------------
}

//Framebuffer generation stuf
void GPU_render::gen_fbo(FBO* fbo){
  //---------------------------

  glGenFramebuffers(1, &fbo->ID_fbo);

  //---------------------------
}
void GPU_render::gen_fbo_tex_color(FBO* fbo){
  vec2 dim = dimManager->get_win_dim();
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  //Create texture
  glGenTextures(1, &fbo->ID_tex_color);
  glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_color);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim.x, dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->ID_tex_color, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //Check
  if(fbo->ID_tex_color == 0){
    cout<<"ERROR ID FBO"<<endl;
  }

  //---------------------------
}
void GPU_render::gen_fbo_tex_color_multisample(FBO* fbo){
  vec2 dim = dimManager->get_win_dim();
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
  vec2 dim = dimManager->get_win_dim();
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

  //Check
  if(fbo->ID_tex_depth == 0){
    cout<<"ERROR ID FBO"<<endl;
  }

  //---------------------------
}
void GPU_render::gen_fbo_check(FBO* fbo){
  //---------------------------

  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);
  auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if(fboStatus != GL_FRAMEBUFFER_COMPLETE){
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete: " << fboStatus << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
Object_* GPU_render::gen_canvas(){
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

//Rendering
void GPU_render::bind_fbo_pass_2_edl(){
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
void GPU_render::bind_fbo_pass_2_inv(){
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
void GPU_render::bind_canvas(){
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

//Update
void GPU_render::update_dim_texture(){
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
void GPU_render::delete_fbo_all(){
  int nb = fbo_vec.size();
  //---------------------------

  for(int i=0; i<nb; i++){
    glDeleteFramebuffers(1, &fbo_vec[i]->ID_fbo);
    delete fbo_vec[i];
  }

  //---------------------------
}
