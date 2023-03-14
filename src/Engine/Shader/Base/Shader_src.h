#ifndef SHADER_SOURCE_H
#define SHADER_SOURCE_H

#include "Shader_obj.h"

#include "../../../common.h"


class Shader_src
{
public:
  Shader_src();
  ~Shader_src();

public:
  void use();

  virtual void setup_shader(Shader_obj* shader){cout<<"Could not setup shader Base class"<<endl;}
  virtual void update_shader(){cout<<"Could not update shader Base class"<<endl;}

  inline string get_name(){return name;}
  inline string get_path_vs(){return path_vs;}
  inline string get_path_fs(){return path_fs;}

protected:
  Shader_obj* shader;

  string name;
  string path_vs;
  string path_fs;
};

#endif
