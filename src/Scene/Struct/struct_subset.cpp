#include "struct_subset.h"


Subset::Subset(){
  //---------------------------

  this->obj_type = "subset";
  this->has_intensity = false;
  this->has_timestamp = false;

  //---------------------------
}

Frame* Subset::get_frame(){
  //---------------------------

  return &frame;

  //---------------------------
}
