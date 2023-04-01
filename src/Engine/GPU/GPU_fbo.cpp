#include "GPU_fbo.h"

#include "../Node_engine.h"
#include "../Core/Dimension.h"


//Constructor / Destructor
GPU_fbo::GPU_fbo(Node_engine* node_engine){
  //---------------------------

  this->dimManager = node_engine->get_dimManager();

  //---------------------------
}
GPU_fbo::~GPU_fbo(){
  //---------------------------

  //---------------------------
}

//Create / remove fbo
void GPU_fbo::init_create_rendering_fbo(){
  //---------------------------

  //Create pyramid FBO
  this->struct_pyramid = new Pyramid();
  this->struct_pyramid->nb_lvl = 5;
  this->struct_pyramid->size_nn.push_back(1);
  this->struct_pyramid->size_nn.push_back(2);
  this->struct_pyramid->size_nn.push_back(4);
  this->struct_pyramid->size_nn.push_back(8);
  this->struct_pyramid->size_nn.push_back(16);
  this->struct_pyramid->size_nn.push_back(32);
  this->gen_fbo_tex_sector_idx();

  for(int i=0; i<struct_pyramid->nb_lvl; i++){
    FBO* fbo = create_new_pyramid_fbo("fbo_py_lvl_" + to_string(i));
    struct_pyramid->fbo_vec.push_back(fbo);
  }
  this->create_new_fbo("fbo_py_visibility");

  //Create specific FBO
  this->create_new_fbo("fbo_pass_1");
  this->create_new_fbo("fbo_recombination");
  this->create_new_fbo("fbo_edl");

  //Create GFBO
  this->create_gbuffer();

  //---------------------------
}
void GPU_fbo::delete_fbo_all(){
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
FBO* GPU_fbo::create_new_fbo(string name){
  FBO* fbo = new FBO();
  //---------------------------

  fbo->name = name;
  this->gen_fbo(fbo);
  this->gen_fbo_tex_color(fbo, 0);
  this->gen_fbo_tex_depth(fbo);
  this->gen_fbo_check(fbo);
  this->fbo_vec.push_back(fbo);

  //---------------------------
  return fbo;
}
FBO* GPU_fbo::create_new_pyramid_fbo(string name){
  FBO* fbo = new FBO();
  //---------------------------

  fbo->name = name;
  this->gen_fbo(fbo);
  this->gen_fbo_tex_color(fbo, 0);
  this->gen_fbo_tex_position(fbo, 1);
  this->gen_fbo_tex_depth(fbo);
  this->gen_fbo_attachment(fbo, 2);
  this->gen_fbo_check(fbo);
  this->fbo_vec.push_back(fbo);

  //---------------------------
  return fbo;
}
void GPU_fbo::create_gbuffer(){
  FBO* gfbo = new FBO();
  //---------------------------

  gfbo->name = "fbo_geometry";
  this->gen_fbo(gfbo);
  this->gen_fbo_tex_color(gfbo, 0);
  this->gen_fbo_tex_position(gfbo, 1);
  this->gen_fbo_tex_normal(gfbo, 2);
  this->gen_fbo_tex_depth(gfbo);
  this->gen_fbo_attachment(gfbo, 3);
  this->gen_fbo_check(gfbo);

  //---------------------------
  this->fbo_vec.push_back(gfbo);
}

//FBO generation
void GPU_fbo::gen_fbo(FBO* fbo){
  //---------------------------

  glGenFramebuffers(1, &fbo->ID_fbo);

  //---------------------------
}
void GPU_fbo::gen_fbo_check(FBO* fbo){
  //---------------------------

  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);
  auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if(fboStatus != GL_FRAMEBUFFER_COMPLETE){
    std::cout << "ERROR::FRAMEBUFFER:: GPU_fbo is not complete: " << fboStatus << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void GPU_fbo::gen_fbo_attachment(FBO* fbo, int nb_attachment){
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  if(nb_attachment == 1){
    unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);
  }
  else if(nb_attachment == 2){
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
  }
  else if(nb_attachment == 3){
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
  }

  //Unbind
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}

//FBO texture generation
void GPU_fbo::gen_fbo_tex_color(FBO* fbo, int attach_id){
  vec2 dim = dimManager->get_gl_dim();
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  //Create texture
  glGenTextures(1, &fbo->ID_tex_color);
  glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_color);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.x, dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attach_id, GL_TEXTURE_2D, fbo->ID_tex_color, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //Check
  if(fbo->ID_tex_color == 0){
    cout<<"[error] FBO color ID"<<endl;
  }

  //---------------------------
}
void GPU_fbo::gen_fbo_tex_color_multisample(FBO* fbo, int attach_id){
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
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attach_id, GL_TEXTURE_2D_MULTISAMPLE, fbo->ID_tex_color, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void GPU_fbo::gen_fbo_tex_position(FBO* fbo, int attach_id){
  vec2 dim = dimManager->get_gl_dim();
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  //Create texture
  glGenTextures(1, &fbo->ID_tex_position);
  glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_position);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, dim.x, dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attach_id, GL_TEXTURE_2D, fbo->ID_tex_position, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //Check
  if(fbo->ID_tex_position == 0){
    cout<<"[error] FBO position ID"<<endl;
  }

  //---------------------------
}
void GPU_fbo::gen_fbo_tex_normal(FBO* fbo, int attach_id){
  vec2 dim = dimManager->get_gl_dim();
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  //Create texture
  glGenTextures(1, &fbo->ID_tex_normal);
  glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_normal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, dim.x, dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attach_id, GL_TEXTURE_2D, fbo->ID_tex_normal, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //Check
  if(fbo->ID_tex_normal == 0){
    cout<<"[error] FBO normal ID"<<endl;
  }

  //---------------------------
}
void GPU_fbo::gen_fbo_tex_pyramide(FBO* fbo, int attach_id){
  vec2 dim = dimManager->get_gl_dim();
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  //Create texture
  glGenTextures(1, &fbo->ID_tex_normal);
  glBindTexture(GL_TEXTURE_2D, fbo->ID_tex_normal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.x, dim.y, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attach_id, GL_TEXTURE_2D, fbo->ID_tex_normal, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //Check
  if(fbo->ID_tex_normal == 0){
    cout<<"[error] FBO normal ID"<<endl;
  }

  //---------------------------
}
void GPU_fbo::gen_fbo_tex_depth(FBO* fbo){
  vec2 dim = dimManager->get_gl_dim();
  //---------------------------

  //Bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID_fbo);

  //Creat texture
  glGenTextures(1, &fbo->ID_buffer_depth);
  glBindTexture(GL_TEXTURE_2D, fbo->ID_buffer_depth);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dim.x, dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->ID_buffer_depth, 0);

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //Check
  if(fbo->ID_buffer_depth == 0){
    cout<<"[error] FBO depth ID"<<endl;
  }

  //---------------------------
}
void GPU_fbo::gen_fbo_tex_sector_idx(){
  //---------------------------

  // Create a correspondence table texture
  glGenTextures(1, &struct_pyramid->tex_index);
  glBindTexture(GL_TEXTURE_1D, struct_pyramid->tex_index);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Create a buffer to hold the table data
  vector<float> index_vec;
  for(float i=-1; i<2; i++){
    for(float j=-1; j<2; j++){
      float index;
      if(i == 0 && j== 0){
        index = -1;
      }else{
        float alpha = atan(i, j);

        if(alpha < 0){
          alpha = alpha + 2 * M_PI;
        }

        float theta = (8 * alpha) / (2 * M_PI);
        index = round(theta);
      }

      index_vec.push_back(index);
    }
  }

  // Upload the table data to the texture
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, index_vec.size(), 0, GL_RED, GL_FLOAT, index_vec.data());

  //Unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  //---------------------------
}

//Accesseur
FBO* GPU_fbo::get_fbo_byName(string querry){
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
