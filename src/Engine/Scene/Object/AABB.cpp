#include "AABB.h"


//Constructor / destructor
AABB::AABB(){
  //---------------------------

  this->aabb_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

  //---------------------------
}
AABB::~AABB(){}

Glyph* AABB::obj_aabb(){
  Glyph* aabb = new Glyph();
  //---------------------------

  //Create glyph
  aabb->name = "aabb";
  aabb->draw_width = 1;
  aabb->visibility = true;
  aabb->draw_type = "line";
  aabb->permanent = true;
  aabb->color_unique = aabb_color;

  for(int i=0; i<24; i++){
    aabb->color.push_back(aabb_color);
  }

  //---------------------------
  return aabb;
}

void AABB::update_aabb(Glyph* aabb, Cloud* cloud){
  vector<vec3>& XYZ = aabb->location;
  vector<vec4>& RGB = aabb->color;
  vec3 min = cloud->min;
  vec3 max = cloud->max;
  //---------------------------

  //location
  aabb->location = build_box(min, max);

  //---------------------------
}
void AABB::update_aabb(Glyph* aabb, Subset* subset){
  vector<vec3>& XYZ = aabb->location;
  vector<vec4>& RGB = aabb->color;
  vec3 min = subset->min;
  vec3 max = subset->max;
  //---------------------------

  //location
  aabb->location = build_box(min, max);

  //---------------------------
}

vector<vec3> AABB::build_box(vec3 min, vec3 max){
  vector<vec3> XYZ;
  vec3 l1, l2;
  //---------------------------

  for(int i=0; i<3; i++){
    l1=min;
    l2=min;
    l2[i]=max[i];
    XYZ.push_back(l1);
    XYZ.push_back(l2);

    l1=max;
    l2=max;
    l2[i]=min[i];
    XYZ.push_back(l1);
    XYZ.push_back(l2);
  }
  for(int i=0; i<2; i++){
    l1=min;
    l1.z=max.z;
    l2=min;
    l2.z=max.z;
    l2[i]=max[i];
    XYZ.push_back(l1);
    XYZ.push_back(l2);

    l1=max;
    l1.z=min.z;
    l2=max;
    l2.z=min.z;
    l2[i]=min[i];
    XYZ.push_back(l1);
    XYZ.push_back(l2);

    l1=min;
    l1[i]=max[i];
    l2=l1;
    l2.z=max.z;
    XYZ.push_back(l1);
    XYZ.push_back(l2);
  }

  //---------------------------
  return XYZ;
}
