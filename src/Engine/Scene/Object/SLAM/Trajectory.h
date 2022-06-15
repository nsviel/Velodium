#ifndef OBJECT_TRAJECTORY_H
#define OBJECT_TRAJECTORY_H

#include "../../../Data/struct_glyph.h"
#include "../../../Data/struct_frame.h"

#include "../../../../common.h"


class Trajectory
{
public:
  //Constructor / Destructor
  Trajectory();
  ~Trajectory();

public:
  void create();
  void update(Cloud*cloud);

  inline Glyph* get_glyph(){return trajectory;}
  inline bool* get_visibility(){return &visibility;}
  inline void set_visibility(bool value){this->visibility = value;}

private:
  Glyph* trajectory;

  bool visibility;
  vec4 color;
  int width;
};

#endif
