#ifndef SHADER_EDL_H
#define SHADER_EDL_H

#include "../../../common.h"

class Dimension;


class Shader_edl
{
public:
  Shader_edl(Dimension* dim);
  ~Shader_edl();

  void setup_shader(GLuint program_ID);
  void update_shader();

  inline string get_path_vs(){return path_vs;}
  inline string get_path_fs(){return path_fs;}
  inline float* get_edl_strength(){return &edl_strength;}
  inline float* get_edl_distance(){return &edl_distance;}
  inline float* get_edl_radius(){return &edl_radius;}
  inline float* get_edl_clip_far(){return &clip_far;}
  inline float* get_edl_clip_near(){return &clip_near;}
  inline bool* get_with_edl(){return &with_edl;}

private:
  Dimension* dimManager;

  string path_vs;
  string path_fs;
  float edl_strength;
  float edl_distance;
  float edl_radius;
  float clip_far;
  float clip_near;
  bool with_edl;
  int program_ID;
};

#endif
