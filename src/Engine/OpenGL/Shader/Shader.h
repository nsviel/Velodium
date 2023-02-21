#ifndef SHADER_H
#define SHADER_H

#include "../../../common.h"

class PP_edl;
class ShaderObject;
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

  inline ShaderObject* get_shader_screen(){return shader_screen;}
  inline ShaderObject* get_shader_render(){return shader_render;}
  inline PP_edl* get_edlManager(){return edlManager;}

private:
  PP_edl* edlManager;
  string shader_dir;
  ShaderObject* shader_screen;
  ShaderObject* shader_render;

};

#endif
