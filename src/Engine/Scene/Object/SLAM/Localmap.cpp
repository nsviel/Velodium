#include "Localmap.h"

#include "../Normal.h"


//Constructor / destructor
Localmap::Localmap(){
  //---------------------------

  this->color = vec4(0.0f, 0.7f, 0.9f, 1.0f);
  this->visibility = true;

  //---------------------------
}
Localmap::~Localmap(){}

void Localmap::create_localmap(){
  this->localmap = new Glyph();
  //---------------------------

  //Create glyph
  localmap->name = "localmap";
  localmap->draw_size = 5;
  localmap->draw_type = "point";
  localmap->color_unique = color;
  localmap->visibility = visibility;

  //---------------------------
}
void Localmap::update_localmap(){
  //---------------------------

  vector<vec3>& XYZ_key = localmap->location;
  vector<vec4>& RGB_key = localmap->color;

  //Construct glyph
  RGB_key.clear();
  for(int i=0; i<XYZ_key.size(); i++){
    RGB_key.push_back(color);
  }

  //---------------------------
}
