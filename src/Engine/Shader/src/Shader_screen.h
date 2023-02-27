#ifndef SHADER_SCREEN_H
#define SHADER_SCREEN_H

#include "../../../common.h"

class Node_engine;
class Configuration;
class Shader_object;


class Shader_screen
{
public:
  Shader_screen(Node_engine* node);
  ~Shader_screen();

  void setup_shader(Shader_object* shader);
  void update_shader();

  inline string get_path_vs(){return path_vs;}
  inline string get_path_fs(){return path_fs;}
  inline bool* get_with_depth(){return &with_depth;}

private:
  Configuration* configManager;

  Shader_object* shader;

  string path_vs;
  string path_fs;
  float clip_far;
  float clip_near;
  bool with_depth;
  int point_size;
};

#endif
