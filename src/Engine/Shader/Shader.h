#ifndef SHADER_H
#define SHADER_H

#include "../../common.h"

class Node_engine;
class Shader_obj;
class Shader_src;


class Shader
{
public:
  Shader(Node_engine* node);
  ~Shader();

public:
  //Main function
  void init_shader();
  void update_shader();
  void use_shader(string shader_name);

  //Accesseur
  Shader_obj* get_shader_obj_byName(string querry);
  Shader_src* get_shader_src_byName(string querry);

  inline int get_nb_shader(){return list_shader_obj->size();}

private:
  Node_engine* node_engine;

  list<Shader_obj*>* list_shader_obj;
  list<Shader_src*>* list_shader_src;

  string shader_dir;
};

#endif
