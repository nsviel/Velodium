#include "Renderer.h"

#include "Pass/Render_pass_1.h"
#include "Pass/Render_pass_2.h"
#include "Pass/Render_pass_3.h"

#include "../Node_engine.h"
#include "../Core/Dimension.h"

//Pass 1: render-to-texture
//Pass 2: texture post-processing
//Pass 3: texture-to-screen


//Constructor / Destructor
Renderer::Renderer(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();
  this->gpu_fbo = node_engine->get_gpu_fbo();

  this->render_pass_1 = new Render_pass_1(node_engine);
  this->render_pass_2 = new Render_pass_2(node_engine);
  this->render_pass_3 = new Render_pass_3(node_engine);

  gpu_fbo->init_create_rendering_fbo();

  //---------------------------
}
Renderer::~Renderer(){
  //---------------------------

  gpu_fbo->delete_fbo_all();
  delete gpu_fbo;

  //---------------------------
}

//Main function
void Renderer::loop_rendering(){
  //---------------------------

  render_pass_1->compute_pass();
  render_pass_2->compute_pass();
  render_pass_3->compute_pass();

  //---------------------------
}

//Subfunction
void Renderer::update_dim_texture(){
  vector<FBO*> fbo_vec = gpu_fbo->get_fbo_vec();
  //---------------------------

  vec2 dim = dimManager->get_gl_dim();
  for(int i=0; i<fbo_vec.size(); i++){
    FBO* fbo = fbo_vec[i];
    glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.x, dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    if(fbo->ID_tex_position != 0){
      glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_position);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, dim.x, dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
    }

    if(fbo->ID_tex_normal != 0){
      glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_normal);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, dim.x, dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
    }

    glBindTexture(GL_TEXTURE_2D, fbo->ID_buffer_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dim.x, dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  }

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  //---------------------------
}
vec4* Renderer::get_screen_color(){
  //---------------------------

  vec4* color = render_pass_1->get_screen_color();

  //---------------------------
  return color;
}
