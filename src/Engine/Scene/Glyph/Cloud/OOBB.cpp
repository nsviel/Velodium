#include "OOBB.h"


//Constructor / destructor
OOBB::OOBB(){
  //---------------------------

  this->color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
  this->width = 1;

  //---------------------------
}
OOBB::~OOBB(){}

Glyph* OOBB::create_oobb(){
  Glyph* oobb = new Glyph();
  //---------------------------

  //Create glyph
  oobb->name = "OOBB";
  oobb->draw_width = width;
  oobb->visibility = false;
  oobb->draw_type = "line";
  oobb->permanent = false;
  oobb->color_unique = color;

  //Box color
  for(int i=0; i<24; i++){
    oobb->color.push_back(color);
  }

  //Arrow color
  for(int i=0; i<6; i++){
    oobb->color.push_back(vec4(0,0,1,1));
  }

  //---------------------------
  return oobb;
}
void OOBB::update_oobb(Glyph* oobb, mat4 transformation){
  vec3 min = vec3(-0.5f, -0.5f, -0.5f);
  vec3 max = vec3(0.5f, 0.5f, 0.5f);
  //---------------------------

  //location
  vector<vec3> box = build_box(min, max);
  for(int i=0; i<box.size(); i++){
    vec4 point = vec4(box[i].x, box[i].y, box[i].z, 1.0f);
    point = point * transformation;
    box[i] = vec3(point.x, point.y, point.z);
  }

  oobb->location = box;

  //---------------------------
}
vector<vec3> OOBB::build_box(vec3 min, vec3 max){
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

  //Arrow direction
  l1 = vec3(min.x*3/4, (max.y+min.y)/2, max.z);
  l2 = vec3(max.x*3/4, (max.y+min.y)/2, max.z);
  XYZ.push_back(l1);
  XYZ.push_back(l2);

  l1 = vec3(max.x*3/4, (max.y+min.y)/2, max.z);
  l2 = vec3(max.x*3/4-max.x/4, (max.y+min.y)/2-max.y/4, max.z);
  XYZ.push_back(l1);
  XYZ.push_back(l2);

  l1 = vec3(max.x*3/4, (max.y+min.y)/2, max.z);
  l2 = vec3(max.x*3/4-max.x/4, (max.y+min.y)/2+max.y/4, max.z);
  XYZ.push_back(l1);
  XYZ.push_back(l2);

  //---------------------------
  return XYZ;
}
