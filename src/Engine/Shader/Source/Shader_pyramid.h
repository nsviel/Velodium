#ifndef SHADER_PYRAMID_H
#define SHADER_PYRAMID_H

#include "../Base/Shader_src.h"

#include "../../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class Shader_obj;


class Shader_pyramid : public Shader_src
{
public:
  Shader_pyramid(Node_engine* node);
  ~Shader_pyramid();

  void setup_shader(Shader_obj* shader_lvl_0, Shader_obj* shader_lvl, Shader_obj* shader_visibility);
  void update_shader();

private:
  Configuration* configManager;
  Dimension* dimManager;

  Shader_obj* shader_lvl_0;
  Shader_obj* shader_lvl_n;
  Shader_obj* shader_visibility;

  float clip_far;
  float clip_near;
};

#endif
