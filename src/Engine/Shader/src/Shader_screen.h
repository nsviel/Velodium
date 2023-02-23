#ifndef SHADER_SCREEN_H
#define SHADER_SCREEN_H

#include "../../../common.h"


class Shader_screen
{
public:
  Shader_screen();
  ~Shader_screen();

  void setup_shader(GLuint program_ID);
  void update_shader();

  inline string get_path_vs(){return path_vs;}
  inline string get_path_fs(){return path_fs;}
  inline bool* get_with_depth(){return &with_depth;}

private:
  string path_vs;
  string path_fs;
  bool with_depth;
  int program_ID;
};

#endif
