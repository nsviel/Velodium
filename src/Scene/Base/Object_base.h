#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "Glyph_base.h"
#include "../../common.h"
#include "../../Scene/Struct/struct_glyph.h"
#include "../../Scene/Struct/struct_subset.h"


class Object_base
{
public:
  //Constructor / Destructor
  Object_base();
  ~Object_base();

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
