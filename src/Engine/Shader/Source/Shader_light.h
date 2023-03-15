#ifndef SHADER_LIGHT_H
#define SHADER_LIGHT_H

#include "../Base/Shader_src.h"

#include "../../../common.h"

class Configuration;
class Shader_obj;


class Shader_light : public Shader_src
{
public:
  Shader_light();
  ~Shader_light();

  void setup_shader(Shader_obj* shader_lamp, Shader_obj* shader_light);
  void update_shader();

  inline vec3 get_light_color(){return light_color;}
  inline void set_light_color(vec3 value){light_color=value;update_shader();}
  inline void set_light_position(vec3 value){light_position=value;update_shader();}

private:
  Configuration* configManager;

  Shader_obj* shader_light;
  Shader_obj* shader_lamp;
  vec3 light_color;
  vec3 light_position;
};

#endif
