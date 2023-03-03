#include "Matching.h"


//Constructor / destructor
Matching::Matching(){
  //---------------------------

  this->name = "matching";
  this->color = vec4(0.11f, 0.35f, 0.69f, 1.0f);

  //---------------------------
  this->create_glyph();
}
Matching::~Matching(){}

void Matching::create_glyph(){
  this->matching = new Glyph();
  //---------------------------

  matching->name = "matching";
  matching->draw_line_width = 100;
  matching->draw_type_name = "line";
  matching->unicolor = color;
  matching->is_visible = true;
  matching->is_permanent = true;

  //---------------------------
}
void Matching::update_matching(vector<vec3>& xyz){
  vector<vec4>& rgb = matching->rgb;
  matching->xyz = xyz;
  //---------------------------

  rgb.clear();
  for(int i=0; i<xyz.size(); i++){
    rgb.push_back(matching->unicolor);
  }

  //---------------------------
}
void Matching::reset(){
  //---------------------------

  matching->is_visible = false;

  //---------------------------
}
