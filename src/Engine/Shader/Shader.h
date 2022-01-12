#ifndef SHADER_H
#define SHADER_H

#include "../../common.h"

class PP_edl;
class ShaderObject;

class Shader
{
public:
  Shader();
  ~Shader();

public:
  void init();
  void use(string shader_name);
  void use_scene();
  void use_screen();

  inline ShaderObject* get_shader_scene(){return shader_scene;}
  inline ShaderObject* get_shader_screen(){return shader_screen;}
  inline PP_edl* get_edlManager(){return edlManager;}

private:
  PP_edl* edlManager;
  string shader_dir;
  ShaderObject* shader_scene;
  ShaderObject* shader_screen;

};

#endif
