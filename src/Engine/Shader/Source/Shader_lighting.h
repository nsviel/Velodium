#ifndef SHADER_LIGHTING_H
#define SHADER_LIGHTING_H

#include "../Base/Shader_src.h"

#include "../../../common.h"

class Configuration;
class Shader_obj;


class Shader_lighting : public Shader_src
{
public:
  Shader_lighting();
  ~Shader_lighting();

  void setup_shader(Shader_obj* shader_lamp, Shader_obj* shader_lighting);
  void update_shader();

  inline vec3 get_light_color(){return light_color;}
  inline void set_light_color(vec3 color){light_color=color;}

private:
  Configuration* configManager;

  Shader_obj* shader_lighting;
  Shader_obj* shader_lamp;
  vec3 light_color;
};

#endif
