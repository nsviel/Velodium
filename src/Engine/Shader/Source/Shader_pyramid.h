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

  void setup_shader(Shader_obj* shader);
  void update_shader();

private:
  Configuration* configManager;
  Dimension* dimManager;

  float clip_far;
  float clip_near;
};

#endif
