#include "Matching.h"


//Constructor / destructor
Matching::Matching(){
  //---------------------------

  this->color = vec4(0.0f, 0.7f, 0.9f, 1.0f);
  this->visibility = true;

  //---------------------------
}
Matching::~Matching(){}

void Matching::create_keypoint_subset(Subset* subset){
  Glyph keypoint;
  //---------------------------

  //Create glyph
  keypoint.name = "keypoint";
  keypoint.draw_size = 5;
  keypoint.draw_type = "point";
  keypoint.color_unique = color;
  keypoint.visibility = visibility;

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
