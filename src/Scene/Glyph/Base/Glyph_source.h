#ifndef GLYPH_SOURCE_H
#define GLYPH_SOURCE_H

#include "../../Struct/struct_glyph.h"
#include "../../Struct/struct_subset.h"
#include "../../../common.h"


class Glyph_source
{
public:
  //Constructor / Destructor
  Glyph_source();
  ~Glyph_source();

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
