#include "GPU_rendering.h"
#include "GPU_data.h"
#include "GPU_pyramid.h"

#include "../Shader/Base/Shader_obj.h"
#include "../Node_engine.h"
#include "../Core/Dimension.h"
#include "../Core/Configuration.h"
#include "../Core/Engine.h"
#include "../Camera/Camera.h"
#include "../Shader/Shader.h"

#include <filesystem>
#include <FreeImage.h>
#include <cstdint>


//Constructor / Destructor
GPU_rendering::GPU_rendering(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();
  this->shaderManager = node_engine->get_shaderManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->engineManager = node_engine->get_engineManager();
  this->configManager = new Configuration();
  this->gpuManager = new GPU_data();
  this->fboManager = node_engine->get_fboManager();
  this->pyramidManager = new GPU_pyramid(node_engine);

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
  bool is_timer = false;
  if(is_timer) tic();
  //---------------------------

  //Enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //-------------------------------

  //Get camera matrices
  mat4 mvp = cameraManager->compute_cam_mvp();
  mat4 view = cameraManager->compute_cam_view();
  mat4 proj = cameraManager->compute_cam_proj();

  //Bind first pass fbo
  FBO* fbo_pass_1 = fboManager->get_fbo_byName("fbo_pass_1");
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_pass_1->ID_fbo);

  //Clear framebuffer and enable depth
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Light
  /*Shader_obj* shader_lamp = shaderManager->get_shader_obj_byName("shader_lamp");
  shader_lamp->use();
  shader_lamp->setMat4("MVP", mvp);
  engineManager->draw_light();*/

  //Untextured glyphs
  Shader_obj* shader_untextured = shaderManager->get_shader_obj_byName("shader_mesh_untextured");
  shader_untextured->use();
  shader_untextured->setMat4("MVP", mvp);
  engineManager->draw_untextured_glyph();

  //Textured cloud drawing
  Shader_obj* shader_textured = shaderManager->get_shader_obj_byName("shader_mesh_textured");
  shader_textured->use();
  shader_textured->setMat4("MVP", mvp);
  engineManager->draw_textured_cloud();

  //-------------------------------
  //Bind gfbo
  FBO* gfbo = fboManager->get_fbo_byName("fbo_geometry");
  glBindFramebuffer(GL_FRAMEBUFFER, gfbo->ID_fbo);

  //Clear framebuffer and enable depth
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Untextured cloud
  Shader_obj* shader_geometry = shaderManager->get_shader_obj_byName("shader_geometry");
  vec2 gl_pos = dimManager->get_gl_pos();
  shader_geometry->use();
  shader_geometry->setMat4("VIEW", view);
  shader_geometry->setMat4("PROJ", proj);
  shader_geometry->setVec2("GL_POS", gl_pos);
  engineManager->draw_untextured_cloud();

  //---------------------------
  if(is_timer) toc_us("pass_1");
}
void GPU_rendering::loop_pass_2(){
  bool is_timer = false;
  if(is_timer) tic();
  //---------------------------

  //Disable depth test
  glDisable(GL_DEPTH_TEST);

  //Pyramid
  pyramidManager->bind_pyramid(canvas_render);

  //Recombinaison
  shaderManager->use_shader("shader_recombination");
  this->bind_fbo_pass_2_recombination();

  //EDL shader
  shaderManager->use_shader("shader_edl");
  this->bind_fbo_pass_2_edl();

  //Draw screen quad
  shaderManager->use_shader("shader_canvas");
  this->bind_canvas();

  //---------------------------
  if(is_timer) toc_us("pass_2");
}

//Rendering
void GPU_rendering::bind_fbo_pass_2_recombination(){;
  FBO* fbo_recombination = fboManager->get_fbo_byName("fbo_recombination");
  FBO* fbo_pass_1 = fboManager->get_fbo_byName("fbo_pass_1");
  FBO* fbo_visibility = fboManager->get_fbo_byName("fbo_py_visibility");
  //---------------------------

  //DEBUGING
  Pyramid* struct_pyramid = fboManager->get_struct_pyramid();
  FBO* fbo_lvl_0 = fboManager->get_fbo_byName("fbo_py_lvl_0");
  FBO* gfbo = fboManager->get_fbo_byName("fbo_geometry");

  //Activate depth buffering
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_ALWAYS);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_recombination->ID_fbo);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_tex_position);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_pass_1->ID_tex_color);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, gfbo->ID_buffer_depth);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, fbo_pass_1->ID_buffer_depth);

  gpuManager->draw_object(canvas_render);
  this->unbind_fboAndTexture(4);

  //Disable depth test
  glDisable(GL_DEPTH_TEST);

  //---------------------------
}
void GPU_rendering::bind_fbo_pass_2_edl(){
  FBO* gfbo = fboManager->get_fbo_byName("fbo_geometry");
  FBO* fbo_edl = fboManager->get_fbo_byName("fbo_edl");
  Pyramid* struct_pyramid = fboManager->get_struct_pyramid();
  FBO* fbo_pyr = struct_pyramid->fbo_vec[0];
  vec2 dim = dimManager->get_win_dim();
  FBO* fbo_recombination = fboManager->get_fbo_byName("fbo_recombination");
  //---------------------------

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_edl->ID_fbo);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_recombination->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_recombination->ID_buffer_depth);

  gpuManager->draw_object(canvas_render);
  this->unbind_fboAndTexture(2);

  //---------------------------
}
void GPU_rendering::bind_canvas(){
  FBO* gfbo = fboManager->get_fbo_byName("fbo_geometry");
  FBO* fbo_edl = fboManager->get_fbo_byName("fbo_edl");
  //---------------------------

  //PROBLEM ICI on essaye de mapper texture edl de dim WIN sur canvas de dim GL

  //Bind fbo and clear old one
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_edl->ID_tex_color);

  //Draw quad
  gpuManager->draw_object(canvas_screen);
  this->unbind_fboAndTexture(1);

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

    if(fbo->ID_tex_position != 0){
      glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_position);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, dim.x, dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
    }

    if(fbo->ID_tex_normal != 0){
      glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_normal);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, dim.x, dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
    }

    glBindTexture(GL_TEXTURE_2D, fbo->ID_buffer_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dim.x, dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
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

//Subfunction
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
void GPU_rendering::unbind_fboAndTexture(int nb_tex){
  //---------------------------

  //Unbind texture
  for(int i=0; i<nb_tex; i++){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  //Unbind FBO
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
