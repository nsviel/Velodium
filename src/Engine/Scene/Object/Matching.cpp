#include "Matching.h"


//Constructor / destructor
Matching::Matching(){
  //---------------------------

  this->color = vec4(1.0f, 0.0f, 0.0f, 1.0f);

  //---------------------------
}
Matching::~Matching(){}

void Matching::create_keypoint_subset(Subset* subset){
  Glyph keypoint;
  //---------------------------

  //Create glyph
  keypoint.name = "keypoint";
  keypoint.draw_size = 5;
  keypoint.draw_width = 5;
  keypoint.draw_type = "point";
  keypoint.color_unique = color;
  keypoint.visibility = true;

  //---------------------------
  subset->keypoint = keypoint;
}
void Matching::update_keypoint_subset(Subset* subset){
  Glyph* keypoint = &subset->keypoint;
  //---------------------------

  vector<vec3>& XYZ_key = keypoint->location;
  vector<vec4>& RGB_key = keypoint->color;

  //Construct glyph
  RGB_key.clear();
  for(int i=0; i<XYZ_key.size(); i++){
    RGB_key.push_back(color);
  }

  //---------------------------
}
