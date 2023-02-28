#ifndef SHADER_H
#define SHADER_H

#include "../../common.h"

class Node_engine;
class Shader_object;
class Shader_source;


class Shader
{
public:
  Shader(Node_engine* node);
  ~Shader();

public:
  //Main function
  void init_shader_objects();
  void update_shader_objects();
  void use_shader(string shader_name);

  //Accesseur
  Shader_object* get_shader_obj_byName(string querry);
  Shader_source* get_shader_src_byName(string querry);

private:
  Node_engine* node_engine;

  list<Shader_object*>* list_shader_obj;
  list<Shader_source*>* list_shader_src;

  string shader_dir;
};

#endif
