#ifndef MOD_SHADER_H
#define MOD_SHADER_H

#include "../../../common.h"

class Node_engine;
class Shader;
class PP_edl;


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
  void parameter();

private:
  Shader* shaderManager;
  PP_edl* edlManager;

  int item_width;
};

#endif
