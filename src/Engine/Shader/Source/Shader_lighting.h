#ifndef SHADER_LIGHTING_H
#define SHADER_LIGHTING_H

#include "../Base/Shader_source.h"

#include "../../../common.h"

class Configuration;
class Shader_object;


class Shader_lighting : public Shader_source
{
public:
  Shader_lighting();
  ~Shader_lighting();

  void setup_shader(Shader_object* shader);
  void update_shader();

  inline vec3 get_light_color(){return light_color;}
  inline void set_light_color(vec3 color){light_color=color;}

private:
  Configuration* configManager;

  vec3 light_color;
};

#endif
