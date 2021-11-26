#include "Axis.h"


//Constructor / destructor
Axis::Axis(){
  //---------------------------


  //---------------------------
}
Axis::~Axis(){}

Glyph* Axis::obj_axis(){
  Glyph* axis = new Glyph();
  //---------------------------

  //Create glyph
  axis->name = "axis";
  axis->draw_width = 3;
  axis->visibility = true;
  axis->draw_type = "line";
  axis->permanent = true;

  //Parameters
  vector<vec3>& XYZ = axis->location;
  vector<vec4>& RGB = axis->color;

  //Construct glyph
  //X axis
  XYZ.push_back(vec3(0, 0, 0));
  XYZ.push_back(vec3(1, 0, 0));
  RGB.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
  RGB.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));

  //Y axis
  XYZ.push_back(vec3(0, 0, 0));
  XYZ.push_back(vec3(0, 1, 0));
  RGB.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));
  RGB.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));

  //Z axis
  XYZ.push_back(vec3(0, 0, 0));
  XYZ.push_back(vec3(0, 0, 1));
  RGB.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
  RGB.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));

  //---------------------------
  return axis;
}
Glyph* Axis::obj_axis_circle(float circleRadius){
  Glyph* axis_circle = new Glyph();
  //---------------------------

  //Create glyph
  axis_circle->name = "axis_circle";
  axis_circle->draw_width = 1;
  axis_circle->visibility = false;
  axis_circle->draw_type = "line";
  axis_circle->permanent = true;

  //Parameters
  int nb_segm = 100;
  vector<vec3>& XYZ = axis_circle->location;
  vector<vec4>& RGB = axis_circle->color;

  //Contruct glyph
  int cpt = 0;
  for(int i=0; i<nb_segm; i++){
    float theta_1 = 2.0f * 3.1415926f * float(i) / float(nb_segm);
    float theta_2 = 2.0f * 3.1415926f * float(i+1) / float(nb_segm);

    //Vertex 1
    float x = circleRadius * cosf(theta_1);
    float y = circleRadius * sinf(theta_1);
    XYZ.push_back(vec3(x, y, 0));
    RGB.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));

    //Vertex 2
    x = circleRadius * cosf(theta_2);
    y = circleRadius * sinf(theta_2);
    XYZ.push_back(vec3(x, y, 0));
    RGB.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
  }

  //---------------------------
  return axis_circle;
}

void Axis::obj_axis_cloud(Subset* subset){
  Glyph axis_cloud;
  //---------------------------

  //Create glyph
  axis_cloud.name = "subset_axis";
  axis_cloud.draw_width = 3;
  axis_cloud.visibility = true;
  axis_cloud.draw_type = "line";

  //Axis colors
  vector<vec4>& RGB = axis_cloud.color;
  RGB.push_back(vec4(0.9f, 0.2f, 0.2f, 1.0f));
  RGB.push_back(vec4(0.9f, 0.2f, 0.2f, 1.0f));
  RGB.push_back(vec4(0.2f, 0.9f, 0.2f, 1.0f));
  RGB.push_back(vec4(0.2f, 0.9f, 0.2f, 1.0f));
  RGB.push_back(vec4(0.2f, 0.2f, 0.9f, 1.0f));
  RGB.push_back(vec4(0.2f, 0.2f, 0.9f, 1.0f));

  //---------------------------
  subset->axis = axis_cloud;
}
void Axis::update_axis_cloud(Subset* subset){
  Glyph* axis_cloud = &subset->axis;
  vector<vec3>& XYZ = axis_cloud->location;
  vec3 COM = subset->COM;
  //---------------------------

  XYZ.clear();

  //Axis rotation
  mat4 rot = subset->rotat;
  vec4 rotX = vec4(0.1,0,0,1) * rot;
  vec4 rotY = vec4(0,0.1,0,1) * rot;
  vec4 rotZ = vec4(0,0,0.1,1) * rot;

  //Construct glyph
  //X Axis
  XYZ.push_back(vec3(COM.x,COM.y,COM.z));
  XYZ.push_back(vec3(COM.x + rotX.x, COM.y + rotX.y, COM.z + rotX.z));

  //Y Axis
  XYZ.push_back(vec3(COM.x,COM.y,COM.z));
  XYZ.push_back(vec3(COM.x + rotY.x, COM.y + rotY.y, COM.z + rotY.z));

  //Z Axis
  XYZ.push_back(vec3(COM.x,COM.y,COM.z));
  XYZ.push_back(vec3(COM.x + rotZ.x, COM.y + rotZ.y, COM.z + rotZ.z));

  //---------------------------
}
