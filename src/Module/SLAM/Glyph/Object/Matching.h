#ifndef SLAM_OBJECT_MATCHING_H
#define SLAM_OBJECT_MATCHING_H

#include "../../../../Scene/Glyph/Base/Glyph_source.h"


class Matching : public Glyph_source
{
public:
  //Constructor / Destructor
  Matching();
  ~Matching();

public:
  void create_glyph();
  void update_matching(vector<vec3>& xyz);
  void reset();

  inline Glyph* get_glyph(){return matching;}

private:
  Glyph* matching;
  vec4 color;
};

#endif
