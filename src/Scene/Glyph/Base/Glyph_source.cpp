#include "Glyph_source.h"


//Constructor / destructor
Glyph_source::Glyph_source(){
  //---------------------------

  this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->visibility = true;

  //---------------------------
}
Glyph_source::~Glyph_source(){
  //---------------------------

  delete glyph;

  //---------------------------
}
