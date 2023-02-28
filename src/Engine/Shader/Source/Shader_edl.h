#ifndef SHADER_EDL_H
#define SHADER_EDL_H

#include "../Object/Shader_source.h"

#include "../../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class Shader_object;


class Shader_edl : public Shader_source
{
public:
  Shader_edl(Node_engine* node);
  ~Shader_edl();

  void setup_shader(Shader_object* shader);
  void update_shader();

  inline float* get_edl_strength(){return &edl_strength;}
  inline float* get_edl_distance(){return &edl_distance;}
  inline float* get_edl_radius(){return &edl_radius;}
  inline float* get_edl_clip_far(){return &clip_far;}
  inline float* get_edl_clip_near(){return &clip_near;}
  inline bool* get_with_edl(){return &with_edl;}
  inline bool* get_with_inv(){return &with_inv;}

private:
  Configuration* configManager;
  Dimension* dimManager;

  float edl_strength;
  float edl_distance;
  float edl_radius;
  float clip_far;
  float clip_near;
  bool with_edl;
  bool with_inv;
  int program_ID;
};

#endif
