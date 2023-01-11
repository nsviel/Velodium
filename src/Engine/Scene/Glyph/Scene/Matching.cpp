#include "Matching.h"

#include "../../../../Specific/fct_math.h"


//Constructor / destructor
Matching::Matching(){
  //---------------------------

  this->color = vec4(0.11f, 0.35f, 0.69f, 1.0f);

  //---------------------------
  this->create_matching();
}
Matching::~Matching(){}

void Matching::create_matching(){
  this->matching = new Glyph();
  //---------------------------

  matching->name = "matching";
  matching->draw_width = 100;
  matching->draw_type = "line";
  matching->color_unique = color;
  matching->visibility = true;
  matching->permanent = true;

  //---------------------------
}
void Matching::update_matching(vector<vec3>& xyz){
  vector<vec4>& rgb = matching->rgb;
  matching->xyz = xyz;
  //---------------------------

  rgb.clear();
  for(int i=0; i<xyz.size(); i++){
    rgb.push_back(matching->color_unique);
  }

  //---------------------------
}
void Matching::reset(){
  //---------------------------

  matching->visibility = false;

  //---------------------------
}
