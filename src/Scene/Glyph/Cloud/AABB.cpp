#include "AABB.h"


AABB::AABB(){
  //---------------------------

  this->name = "aabb";
  this->color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  this->width = 1;

  //---------------------------
  this->create_glyph();
}

//Main function
void AABB::create_glyph(){
  this->glyph = new Glyph();
  //---------------------------

  //Create glyph
  glyph->name = "aabb";
  glyph->draw_line_width = width;
  glyph->is_visible = true;
  glyph->draw_type_name = "line";
  glyph->is_permanent = true;
  glyph->unicolor = color;

  for(int i=0; i<24; i++){
    glyph->rgb.push_back(color);
  }

  //---------------------------
}
void AABB::update_glyph(Collection* collection, Glyph* glyph){
  vec3 min = collection->min;
  vec3 max = collection->max;
  //---------------------------

  //location
  glyph->xyz = build_box(min, max);

  //---------------------------
}
void AABB::update_glyph(Cloud* cloud){
  vector<vec3>& XYZ = glyph->xyz;
  vector<vec4>& RGB = glyph->rgb;
  vec3 min = cloud->min;
  vec3 max = cloud->max;
  //---------------------------

  //location
  glyph->xyz = build_box(min, max);

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
