#ifndef OBJECT_TRAJECTORY_H
#define OBJECT_TRAJECTORY_H

#include "../../../common.h"
#include "../../Data/struct_glyph.h"
#include "../../Data/struct_frame.h"


class Trajectory
{
public:
  //Constructor / Destructor
  Trajectory();
  ~Trajectory();

public:
  void create();
  void update(Cloud*cloud);
  void reset();

  inline Glyph* get_glyph(){return trajectory;}

private:
  Glyph* trajectory;
  vec4 color;
  int width;
};

#endif
