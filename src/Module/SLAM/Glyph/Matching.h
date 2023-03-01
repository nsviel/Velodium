#ifndef SLAM_OBJECT_MATCHING_H
#define SLAM_OBJECT_MATCHING_H

#include "../Base/SLAM_object_base.h"


class Matching : public SLAM_object_base
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
