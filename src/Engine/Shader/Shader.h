#ifndef SHADER_H
#define SHADER_H

#include "../../common.h"

class Shader_edl;
class Shader_screen;
class Shader_object;
class Shader_source;
class Node_engine;


//FAire polymophic shader source system et essayé de faire une list des enfants
//Faire list shader
//Faire list shader source
//Asseccesur shader obj et shader source via name

class Shader
{
public:
  Shader(Node_engine* node);
  ~Shader();

public:
  void init_shader();
  void update_shader();
  void use_shader(string shader_name);

  Shader_object* get_shader_byName(string querry_name);

  inline Shader_edl* get_edlManager(){return edlManager;}
  inline Shader_screen* get_screenManager(){return screenManager;}

private:
  Shader_screen* screenManager;
  Shader_edl* edlManager;

  list<Shader_object*>* list_shader_obj;
  list<Shader_source*>* list_shader_src;
  string shader_dir;
};

#endif
