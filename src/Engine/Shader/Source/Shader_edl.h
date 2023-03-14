#ifndef SHADER_EDL_H
#define SHADER_EDL_H

#include "../Base/Shader_src.h"

#include "../../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class Shader_obj;


class Shader_edl : public Shader_src
{
public:
  Shader_edl(Node_engine* node);
  ~Shader_edl();

  void setup_shader(Shader_obj* shader);
  void update_shader();

  inline float* get_edl_strength(){return &edl_strength;}
  inline float* get_edl_distance(){return &edl_distance;}
  inline float* get_edl_radius(){return &edl_radius;}
  inline float* get_edl_clip_far(){return &clip_far;}
  inline float* get_edl_clip_near(){return &clip_near;}
  inline bool* get_with_edl(){return &with_edl;}

private:
  Configuration* configManager;
  Dimension* dimManager;

  float edl_strength;
  float edl_distance;
  float edl_radius;
  float clip_far;
  float clip_near;
  bool with_edl;
};

#endif
