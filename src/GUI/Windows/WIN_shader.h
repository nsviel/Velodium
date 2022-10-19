#ifndef WIN_SHADER_H
#define WIN_SHADER_H

#include "../../common.h"

class Node_engine;
class Shader;
class PP_edl;


class WIN_shader
{
public:
  //Constructor / Destructor
  WIN_shader(Node_engine* node_engine);
  ~WIN_shader();

public:
  //Main function
  void window_shader();

  //Sub functions
  void parameter();

private:
  Shader* shaderManager;
  PP_edl* edlManager;

  int item_width;
};

#endif
