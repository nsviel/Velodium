#include "Matching.h"

#include "../../../../Specific/fct_maths.h"


//Constructor / destructor
Matching::Matching(){
  //---------------------------

  this->visibility = true;
  this->color = vec4(0.11f, 0.35f, 0.69f, 1.0f);

  //---------------------------
}
Matching::~Matching(){}

void Matching::create_matching_subset(Subset* subset){
  Glyph matching;
  //---------------------------

  //Create glyph
  matching.name = "matching";
  matching.draw_width = 1;
  matching.draw_type = "line";
  matching.color_unique = color;
  matching.visibility = visibility;

  //---------------------------
}
void Matching::update_matching_subset(Subset* subset){
  /*matching->draw_size = size;
  //---------------------------

  vector<vec3>& key_P = mesh_1->registration.keypoints;
  vector<vec3>& trg_Q = mesh_2->registration.trgpoints;
  vector<vec3> XYZ;
  vector<vec4> RGB;
  //---------------------------

  if(key_P.size() != 0 && trg_Q.size() != 0){
    RGB.push_back(vec4(0.8f, 0.8f, 0.8f, 1.0));

    for(int i=0; i<key_P.size(); i++){
      //Location
      vec3 xyz_1(key_P[i]);
      vec3 xyz_2(trg_Q[i]);
      XYZ.push_back(xyz_1);
      XYZ.push_back(xyz_2);

      //Color
      float Red, Green, Blue;
      if(key_P.size()*2+1 != glyph->color.size() || upColor){
        if(match_rdmColor){
          Red = float(rand()%101)/100;
          Green = float(rand()%101)/100;
          Blue = float(rand()%101)/100;
        }else{
          Red = matchingColor[0];
          Green = matchingColor[1];
          Blue = matchingColor[2];
        }

        RGB.push_back(vec4(Red, Green, Blue, 1.0));
        RGB.push_back(vec4(Red, Green, Blue, 1.0));

        this->upColor = false;
        glyph->color = RGB;
        this->update_color(glyph);
      }
    }

    //Update glyph
    glyph->location = XYZ;
    glyph->visibility = matchON;
    this->update_location(glyph);
  }*/

  //---------------------------
}
