#include "struct_cloud.h"


Cloud::Cloud(){
  //---------------------------

  this->obj_type = "cloud";
  this->has_intensity = false;
  this->has_timestamp = false;

  //---------------------------
}

Frame* Cloud::get_frame(){
  //---------------------------

  return &frame;

  //---------------------------
}
