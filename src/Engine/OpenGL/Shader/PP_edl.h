#ifndef PP_EDL_H
#define PP_EDL_H

#include "../../../common.h"

class Dimension;


class PP_edl
{
public:
  PP_edl(Dimension* dim);
  ~PP_edl();

  void setup_edl(GLuint program_ID);
  void setup_textures(GLuint tex_color, GLuint tex_depth);

  inline bool* get_with_edl(){return &with_edl;}
  inline float* get_edl_strength(){return &edl_strength;}
  inline float* get_edl_distance(){return &edl_distance;}
  inline float* get_edl_radius(){return &edl_radius;}
  inline float* get_edl_clip_far(){return &clip_far;}
  inline float* get_edl_clip_near(){return &clip_near;}

private:
  Dimension* dimManager;

  GLuint tex_color_ID;
  GLuint tex_depth_ID;

  bool with_edl;
  float edl_strength;
  float edl_distance;
  float edl_radius;
  float clip_far;
  float clip_near;
};

#endif
