#ifndef SHADER_H
#define SHADER_H

#include "../../common.h"

class Shader_edl;
class Shader_object;
class Dimension;


class Shader
{
public:
  Shader(Dimension* dimManager);
  ~Shader();

public:
  void init_shader();
  void update_shader();
  void use(string shader_name);

  inline Shader_object* get_shader_screen(){return shader_screen;}
  inline Shader_object* get_shader_render(){return shader_render;}
  inline Shader_edl* get_edlManager(){return edlManager;}

private:
  Shader_edl* edlManager;
  string shader_dir;
  Shader_object* shader_screen;
  Shader_object* shader_render;

};

#endif
