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
  vec3 root = subset->root;
  //---------------------------

  XYZ.clear();

  //Axis rotation
  mat4 R = subset->rotat;
  vec4 Rx = vec4(0.1,0,0,1) * R;
  vec4 Ry = vec4(0,0.1,0,1) * R;
  vec4 Rz = vec4(0,0,0.1,1) * R;

  //Construct glyph
  //X Axis
  XYZ.push_back(vec3(root.x,root.y,root.z));
  XYZ.push_back(vec3(root.x + Rx.x, root.y + Rx.y, root.z + Rx.z));

  //Y Axis
  XYZ.push_back(vec3(root.x,root.y,root.z));
  XYZ.push_back(vec3(root.x + Ry.x, root.y + Ry.y, root.z + Ry.z));

  //Z Axis
  XYZ.push_back(vec3(root.x,root.y,root.z));
  XYZ.push_back(vec3(root.x + Rz.x, root.y + Rz.y, root.z + Rz.z));

  //---------------------------
}
