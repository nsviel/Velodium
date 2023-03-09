#include "Framebuffer.h"


//Constructor / Destructor
Framebuffer::Framebuffer(){
  //---------------------------



  //---------------------------
}
Framebuffer::~Framebuffer(){
  //---------------------------

  //---------------------------
}

void Framebuffer::init_create_fbo(int nb_fbo){
  //---------------------------

  for(int i=0; i<nb_fbo; i++){
    FBO* fbo = new FBO();
    this->gen_fbo(fbo);
    this->gen_fbo_tex_color(fbo);
    this->gen_fbo_tex_depth(fbo);
    this->gen_fbo_check(fbo);
    this->fbo_vec.push_back(fbo);
  }

  fbo_vec[0]->name = "pass_2";

  //---------------------------
}
void Framebuffer::delete_fbo_all(){
  int nb = fbo_vec.size();
  //---------------------------

  for(int i=0; i<nb; i++){
    FBO* fbo = fbo_vec[i];
    glDeleteFramebuffers(1, &fbo->ID_fbo);
    delete fbo;
  }

  fbo_vec.clear();

  //---------------------------
}
FBO* Framebuffer::get_fbo_byName(string querry){
  //---------------------------

  for(int i=0; i<fbo_vec.size(); i++){
    FBO* fbo = fbo_vec[i];
    if(fbo->name == querry){
      return fbo;
    }
  }

  //---------------------------
  return nullptr;
}

//Framebuffer generation
void Framebuffer::gen_fbo(FBO* fbo){
  //---------------------------

  glGenFramebuffers(1, &fbo->ID_fbo);

  //---------------------------
}
void Framebuffer::gen_fbo_tex_color(FBO* fbo){
  vec2 dim = vec2(1);
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
void Framebuffer::gen_fbo_tex_color_multisample(FBO* fbo){
  vec2 dim = vec2(1);
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
void Framebuffer::gen_fbo_tex_depth(FBO* fbo){
  vec2 dim = vec2(1);
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
void Framebuffer::gen_fbo_check(FBO* fbo){
  //---------------------------

  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);
  auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if(fboStatus != GL_FRAMEBUFFER_COMPLETE){
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete: " << fboStatus << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
