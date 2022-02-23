#include "Normal.h"


//Constructor / destructor
Normal::Normal(){
  //---------------------------

  this->normal_size = 1;
  this->normal_color = vec4(0.11f, 0.35f, 0.69f, 1.0f);

  //---------------------------
}
Normal::~Normal(){}

void Normal::obj_normal(Subset* subset){
  Glyph normal;
  //---------------------------

  //Create glyph
  normal.name = "normal";
  normal.draw_width = 1;
  normal.draw_size = normal_size;
  normal.draw_type = "line";
  normal.color_unique = normal_color;
  normal.visibility = false;

  //---------------------------
  subset->normal = normal;
}
void Normal::update_normal(Subset* subset){
  Glyph* normal = &subset->normal;
  vector<vec3>& XYZ_n = normal->location;
  vector<vec4>& RGB_n = normal->color;

  vector<vec3>& XYZ = subset->xyz;
  vector<vec3>& N = subset->N;
  //---------------------------

  //Parameter
  float lgt = 0.01 * normal->draw_size;
  XYZ_n.clear();
  RGB_n.clear();

  //Construct glyph
  if(XYZ.size() != 0 && N.size() != 0 && N.size() == XYZ.size()){
    for(int i=0; i<XYZ.size(); i++){
      XYZ_n.push_back(vec3(XYZ[i].x, XYZ[i].y, XYZ[i].z));
      XYZ_n.push_back(vec3(XYZ[i].x + N[i].x * lgt, XYZ[i].y + N[i].y * lgt, XYZ[i].z + N[i].z * lgt));

      RGB_n.push_back(normal->color_unique);
      RGB_n.push_back(normal->color_unique);
    }
  }

  //---------------------------
}
