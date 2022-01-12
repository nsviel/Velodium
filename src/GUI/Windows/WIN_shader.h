#ifndef WIN_SHADER_H
#define WIN_SHADER_H

#include "../../common.h"

class Shader;
class PP_edl;


class WIN_shader
{
public:
  //Constructor / Destructor
  WIN_shader(Shader* shader);
  ~WIN_shader();

public:
  void window_shader();

private:
  Shader* shaderManager;
  PP_edl* edlManager;

  int item_width;
};

#endif
