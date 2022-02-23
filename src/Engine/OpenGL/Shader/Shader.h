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
  void init();
  void update();
  void use(string shader_name);

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
