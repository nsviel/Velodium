#ifndef SHADER_SCREEN_H
#define SHADER_SCREEN_H

#include "../Object/Shader_source.h"

#include "../../../common.h"

class Node_engine;
class Configuration;
class Shader_object;


class Shader_screen : public Shader_source
{
public:
  Shader_screen(Node_engine* node);
  ~Shader_screen();

  void setup_shader(Shader_object* shader);
  void update_shader();

  inline bool* get_with_depth(){return &with_depth;}

private:
  Configuration* configManager;

  float clip_far;
  float clip_near;
  bool with_depth;
  int point_size;
};

#endif
