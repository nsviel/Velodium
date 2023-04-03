#include "Render_pass_2.h"

#include "../Processing/Render_pyramid.h"

#include "../../Node_engine.h"
#include "../../Core/Dimension.h"
#include "../../Camera/Camera.h"
#include "../../Shader/Shader.h"
#include "../../GPU/GPU_data.h"
#include "../../GPU/GPU_render.h"

#include <filesystem>
#include <FreeImage.h>
#include <cstdint>


//Constructor / Destructor
Render_pass_2::Render_pass_2(Node_engine* node_engine){
  //---------------------------

  this->shaderManager = node_engine->get_shaderManager();
  this->pyramidManager = new Render_pyramid(node_engine);
  this->gpu_render = node_engine->get_gpu_render();
  this->gpu_fbo = node_engine->get_gpu_fbo();
  this->gpu_data = new GPU_data();

  //A VIRER PLUS TARD
  this->dimManager = node_engine->get_dimManager();
  this->cameraManager = node_engine->get_cameraManager();

  this->canvas = gpu_render->gen_canvas();

  //---------------------------
}
Render_pass_2::~Render_pass_2(){}

//Main function
void Render_pass_2::compute_pass(){
  //---------------------------

  this->configure_opengl();
  pyramidManager->bind_pyramid(canvas);
  this->render_recombination();
  this->render_edl();

  //---------------------------
}

//Subfunction
void Render_pass_2::configure_opengl(){
  //---------------------------

  //Activate depth buffering
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_ALWAYS);

  //---------------------------
}
void Render_pass_2::render_recombination(){;
  FBO* fbo_recombination = gpu_fbo->get_fbo_byName("fbo_recombination");
  FBO* fbo_pass_1 = gpu_fbo->get_fbo_byName("fbo_pass_1");
  FBO* fbo_visibility = gpu_fbo->get_fbo_byName("fbo_py_visibility");
  //---------------------------

  //Bind shader
  shaderManager->use_shader("shader_recombination");

  //DEBUGING
  Pyramid* struct_pyramid = gpu_fbo->get_struct_pyramid();
  FBO* fbo_lvl_0 = gpu_fbo->get_fbo_byName("fbo_py_lvl_0");
  FBO* gfbo = gpu_fbo->get_fbo_byName("fbo_geometry");

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_recombination->ID_fbo);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_pass_1->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_visibility->ID_tex_color);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, fbo_pass_1->ID_buffer_depth);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, fbo_visibility->ID_buffer_depth);

  //Draw render canvas
  gpu_data->draw_object(canvas);
  gpu_render->unbind_fboAndTexture(4);

  //---------------------------
}
void Render_pass_2::render_edl(){
  FBO* gfbo = gpu_fbo->get_fbo_byName("fbo_geometry");
  FBO* fbo_edl = gpu_fbo->get_fbo_byName("fbo_edl");
  Pyramid* struct_pyramid = gpu_fbo->get_struct_pyramid();
  FBO* fbo_pyr = struct_pyramid->fbo_vec[0];
  FBO* fbo_recombination = gpu_fbo->get_fbo_byName("fbo_recombination");
  //---------------------------

  shaderManager->use_shader("shader_edl");

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_edl->ID_fbo);

  //Input: read textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_recombination->ID_tex_color);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_recombination->ID_buffer_depth);

  //Draw render canvas
  gpu_data->draw_object(canvas);
  gpu_render->unbind_fboAndTexture(2);

  //---------------------------
}


/*vec3 Render_pass_2::fct_unproject(vec2 coord_frag){
  vec2 gl_dim = dimManager->get_win_dim();
  mat4 view = cameraManager->compute_cam_view();
  //---------------------------

  //Raster space to NDC space
  vec2 coord_ndc;
  coord_ndc.x = (coord_frag.x) / gl_dim.x;
  coord_ndc.y = (coord_frag.y) / gl_dim.y;

  //Coord in NDC space to clip coordinate
  vec2 coord_clip;
  coord_clip.x = 2 * coord_ndc.x - 1;
  coord_clip.y = 2 * coord_ndc.y - 1;

  //Clip to view  space
  float ratio = gl_dim.x / gl_dim.y;
  vec4 coord_view;
  coord_view.x = coord_clip.x * tan(65 / 2) * ratio;
  coord_view.y = coord_clip.y * tan(65 / 2);
  coord_view.z = -1;
  coord_view.w = 1;

  //View space to world space
  mat4 view_inv = inverse(view);
  vec4 coord_world = view_inv * coord_view;

  vec3 fct_out  = vec3(coord_world);

  //---------------------------
  return fct_out;
}*/
