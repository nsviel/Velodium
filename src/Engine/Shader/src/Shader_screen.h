#ifndef SHADER_SCREEN_H
#define SHADER_SCREEN_H

#include "../../../common.h"


class Shader_screen
{
public:
  Shader_screen();
  ~Shader_screen();

  void setup_shader(GLuint program_ID);

private:
  bool with_depth;
};

#endif
