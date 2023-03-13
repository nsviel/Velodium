#include "struct_cloud.h"


Cloud::Cloud(){
  //---------------------------

  this->has_intensity = false;
  this->has_timestamp = false;

  //---------------------------
}

void Cloud::reset(){
  //---------------------------

  this->R.clear();
  this->It.clear();
  this->cosIt.clear();
  this->frame.reset();

  //---------------------------
}
Frame* Cloud::get_frame(){
  //---------------------------

  return &frame;

  //---------------------------
}
