#ifndef SLAM_OBJECT_BASE_H
#define SLAM_OBJECT_BASE_H

#include "../../../common.h"
#include "../../../Engine/Data/struct_glyph.h"
#include "../../../Engine/Data/struct_subset.h"


class SLAM_object_base
{
public:
  //Constructor / Destructor
  SLAM_object_base();
  ~SLAM_object_base();

public:
  virtual void create_glyph(){}
  virtual void update_glyph(){}
  virtual void reset_glyph(){}

  inline Glyph* get_glyph(){return glyph;}
  inline vec4* get_color(){return &color;}
  inline bool* get_visibility(){return &visibility;}
  inline void set_visibility(bool value){this->visibility = value;}

protected:
  Glyph* glyph;

  vec4 color;
  bool visibility;
};

#endif
