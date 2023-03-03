#ifndef SLAM_OBJECT_TRAJECTORY_H
#define SLAM_OBJECT_TRAJECTORY_H

#include "../../../../Scene/Glyph/Base/Glyph_source.h"


class Trajectory : public Glyph_source
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
  inline bool* get_visibility(){return &visibility;}
  inline void set_visibility(bool value){this->visibility = value;}

private:
  Glyph* trajectory;

  bool visibility;
  vec4 color;
  int width;
};

#endif