#include "Framebuffer.h"

#include "../Configuration/Dimension.h"


//Constructor / Destructor
Framebuffer::Framebuffer(Dimension* dim){
  //---------------------------

  this->dimManager = dim;


  //---------------------------
}
Framebuffer::~Framebuffer(){}


void Framebuffer::init_rendering_fbo_1(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Init FBO 1
  glGenFramebuffers(1, &fbo_1_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);

  //Init textures
  glGenTextures(1, &tex_color_ID);
  glBindTexture(GL_TEXTURE_2D, tex_color_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gl_dim.x, gl_dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_ID, 0);

  glGenTextures(1, &tex_depth_ID);
  glBindTexture(GL_TEXTURE_2D, tex_depth_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_depth_ID, 0);

  //Debind objects
  glBindTexture(GL_TEXTURE_2D ,0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void Framebuffer::init_rendering_fbo_2(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Init FBO 2
  glGenTextures(1, &tex_edl_ID);
  glGenFramebuffers(1, &fbo_2_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2_ID);

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex_edl_ID);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 64, GL_RGBA, gl_dim.x, gl_dim.y, false);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex_edl_ID, 0);

  //Debind objects
  glBindTexture(GL_TEXTURE_2D ,0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
