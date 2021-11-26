#ifndef DEF_FRAMEBUFFER
#define DEF_FRAMEBUFFER

#include "../../common.h"

class Framebuffer
{
public:
  Framebuffer();
  ~Framebuffer();

public:
  void letsgo();
  void letsmid();
  void letstop();

  void display();
  void create_FBO();
  void create_Texture();
  void create_DepthRenderBuffer();
  void create_bckgrndQuad();
  void readScreenPixels(int width, int height);

private:
  GLuint quadVAO;
  GLuint ID_fbo;
  GLuint ID_texture;
  GLuint ID_depthBuffer;
  int width, height;
};

#endif
