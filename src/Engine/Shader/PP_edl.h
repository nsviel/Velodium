#ifndef PP_EDL_H
#define PP_EDL_H

#include "../../common.h"

class Shader;


class PP_edl
{
public:
  PP_edl();
  ~PP_edl();

  void setup_edl(Shader* shader);
  void setup_textures(GLuint tex_color, GLuint tex_depth);

private:
  GLuint texture_color_ID;
  GLuint texture_depth_ID;
};

#endif
