#ifndef MOD_SHADER_H
#define MOD_SHADER_H

#include "../../../common.h"

class Node_engine;
class Shader;


class MOD_shader
{
public:
  //Constructor / Destructor
  MOD_shader(Node_engine* node_engine);
  ~MOD_shader();

public:
  //Main function
  void window_shader();

  //Sub functions
  void shader_render();
  void shader_lighting();

private:
  Shader* shaderManager;

  int item_width;
};

#endif
