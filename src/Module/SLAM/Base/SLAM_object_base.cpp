#include "SLAM_object_base.h"


//Constructor / destructor
SLAM_object_base::SLAM_object_base(){
  //---------------------------

  this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->visibility = true;

  //---------------------------
}
SLAM_object_base::~SLAM_object_base(){
  //---------------------------

  delete glyph;

  //---------------------------
}
