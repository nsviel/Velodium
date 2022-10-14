#include "Normal.h"

#include "../../../../Specific/fct_maths.h"


//Constructor / destructor
Normal::Normal(){
  //---------------------------

  this->width = 1;
  this->size = 1;
  this->visibility = true;
  this->color = vec4(0.11f, 0.35f, 0.69f, 1.0f);

  //---------------------------
}
Normal::~Normal(){}

void Normal::create_normal_subset(Subset* subset){
  Glyph normal;
  //---------------------------

  //Create glyph
  normal.name = "normal";
  normal.draw_width = width;
  normal.draw_type = "line";
  normal.color_unique = color;
  normal.visibility = visibility;

  //---------------------------
  subset->normal = normal;
}
void Normal::update_normal_subset(Subset* subset){
  Glyph* normal = &subset->normal;
  normal->draw_size = size;
  //---------------------------

  //Get vector values
  vector<vec3>& xyz_s = subset->xyz;
  vector<vec3>& Nxyz_s = subset->N;
  vector<vec3>& xyz_n = normal->location;
  vector<vec4>& rgb_n = normal->color;

  //Check vector length
  if(xyz_s.size() == 0 || Nxyz_s.size() == 0 || Nxyz_s.size() != xyz_s.size()){
    return;
  }

  //Clear old normal values
  xyz_n.clear();
  rgb_n.clear();

  //Construct normal
  float lgt = 0.05 * normal->draw_size;
  for(int i=0; i<xyz_s.size(); i++){
    vec3& xyz = xyz_s[i];
    vec3& nxyz = Nxyz_s[i];

    if(fct_is_nan(nxyz)) continue;

    vec3 vec_n = vec3(xyz.x + nxyz.x * lgt, xyz.y + nxyz.y * lgt, xyz.z + nxyz.z * lgt);

    xyz_n.push_back(xyz);
    xyz_n.push_back(vec_n);

    rgb_n.push_back(normal->color_unique);
    rgb_n.push_back(normal->color_unique);
  }

  //---------------------------
}
