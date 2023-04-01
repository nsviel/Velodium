#include "render_pullpush.h"

#include "../GPU/GPU_data.h"
#include "../GPU/GPU_fbo.h"
#include "../Shader/Base/Shader_obj.h"
#include "../Node_engine.h"
#include "../Shader/Shader.h"
#include "../Camera/Camera.h"


//Constructor / Destructor
render_pullpush::render_pullpush(Node_engine* node_engine){
  //---------------------------



  //---------------------------
}
render_pullpush::~render_pullpush(){}

//Main function
void render_pullpush::bind_pullpush(Object_* canvas){
  //---------------------------



  //---------------------------
}

//Subfunction
void render_pullpush::unbind_fboAndTexture(int nb_tex){
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
