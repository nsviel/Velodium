#include "Object_base.h"


//Constructor / destructor
Object_base::Object_base(){
  //---------------------------

  this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->visibility = true;

  //---------------------------
}
Object_base::~Object_base(){
  //---------------------------

  delete glyph;

  //---------------------------
}
