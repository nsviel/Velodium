#include "Box.h"


//Constructor / destructor
Box::Box(){
  //---------------------------

  this->name = "box";
  this->color = vec4(0.0f, 0.0f, 0.8f, 0.2f);
  this->width = 3;

  //---------------------------
  this->create_glyph();
}
Box::~Box(){
  //---------------------------

  delete box;

  //---------------------------
}

//Main function
void Box::create_glyph(){
  box = new Glyph();
  //---------------------------

  //Create glyph
  box->name = "box";
  box->draw_line_width = width;
  box->is_visible = false;
  box->draw_type_name = "line";
  box->is_permanent = true;
  box->unicolor = color;

  box->min = vec3(0,0,0);
  box->max = vec3(1,1,1);

  this->build_box_location();
  this->build_box_color();

  //---------------------------
}
void Box::update_box(){
  //---------------------------

  box->is_visible = true;

  this->build_box_location();
  this->build_box_color();

  //---------------------------
}

//Subfunction
void Box::build_box_location(){
  vector<vec3>& xyz = box->xyz;
  vec3 min = box->min;
  vec3 max = box->max;
  //---------------------------

  //Clear old data
  xyz.clear();

  //Location
  if(box->draw_type_name == "triangle"){
    xyz.push_back(min);
    xyz.push_back(vec3(max.x, min.y, min.z));
    xyz.push_back(vec3(max.x, max.y, min.z));
    xyz.push_back(min);
    xyz.push_back(vec3(min.x, max.y, min.z));
    xyz.push_back(vec3(max.x, max.y, min.z));
    xyz.push_back(min);
    xyz.push_back(vec3(max.x, min.y, min.z));
    xyz.push_back(vec3(max.x, min.y, max.z));
    xyz.push_back(min);
    xyz.push_back(vec3(min.x, min.y, max.z));
    xyz.push_back(vec3(max.x, min.y, max.z));
    xyz.push_back(min);
    xyz.push_back(vec3(min.x, max.y, min.z));
    xyz.push_back(vec3(min.x, max.y, max.z));
    xyz.push_back(min);
    xyz.push_back(vec3(min.x, min.y, max.z));
    xyz.push_back(vec3(min.x, max.y, max.z));
    xyz.push_back(min);

    xyz.push_back(max);
    xyz.push_back(max);
    xyz.push_back(max);
    xyz.push_back(vec3(max.x, min.y, max.z));
    xyz.push_back(vec3(max.x, min.y, min.z));
    xyz.push_back(max);
    xyz.push_back(vec3(max.x, min.y, min.z));
    xyz.push_back(vec3(max.x, max.y, min.z));
    xyz.push_back(max);
    xyz.push_back(vec3(min.x, max.y, min.z));
    xyz.push_back(vec3(min.x, max.y, max.z));
    xyz.push_back(max);
    xyz.push_back(vec3(max.x, max.y, min.z));
    xyz.push_back(vec3(min.x, max.y, min.z));
    xyz.push_back(max);
    xyz.push_back(vec3(max.x, min.y, max.z));
    xyz.push_back(vec3(min.x, min.y, max.z));
    xyz.push_back(max);
    xyz.push_back(vec3(min.x, max.y, max.z));
    xyz.push_back(vec3(min.x, min.y, max.z));
    xyz.push_back(max);
  }
  else if(box->draw_type_name == "line"){
    vec3 l1, l2;
    for(int i=0; i<3; i++){
      l1=min;
      l2=min;
      l2[i]=max[i];
      xyz.push_back(l1);
      xyz.push_back(l2);

      l1=max;
      l2=max;
      l2[i]=min[i];
      xyz.push_back(l1);
      xyz.push_back(l2);
    }
    for(int i=0; i<2; i++){
      l1=min;
      l1.z=max.z;
      l2=min;
      l2.z=max.z;
      l2[i]=max[i];
      xyz.push_back(l1);
      xyz.push_back(l2);

      l1=max;
      l1.z=min.z;
      l2=max;
      l2.z=min.z;
      l2[i]=min[i];
      xyz.push_back(l1);
      xyz.push_back(l2);

      l1=min;
      l1[i]=max[i];
      l2=l1;
      l2.z=max.z;
      xyz.push_back(l1);
      xyz.push_back(l2);
    }
  }

  //---------------------------
}
void Box::build_box_color(){
  vector<vec4>& rgb = box->rgb;
  //---------------------------

  rgb.clear();

  //Color
  for(int i=0; i<40; i++){
    rgb.push_back(box->unicolor);
  }

  //---------------------------
}
void Box::compute_box_MinMax(Cloud* cloud, vec3 min_perc, vec3 max_perc){
  //---------------------------

  //Get Z extremums
  vec3 min = cloud->min;
  vec3 max = cloud->max;
  vec3 diff = max - min;

  //Compute min abs
  vec3 min_abs;
  vec3 diff_min;
  for(int i=0; i<3; i++){
    if(min_perc[i] > 100) min_perc[i] = 100;
    if(min_perc[i] <= 0) diff_min[i] = 0;
    else diff_min[i] = diff[i] * min_perc[i]/100;

    min_abs[i] = min[i] + diff_min[i];
  }
  box->min = min_abs;

  //Compute max abs
  vec3 max_abs;
  vec3 diff_max;
  for(int i=0; i<3; i++){
    if(max_perc[i] > 100) max_perc[i] = 100;
    if(max_perc[i] <= 0) diff_max[i] = 0;
    else diff_max[i] = diff[i] * max_perc[i]/100;

    max_abs[i] = min[i] + diff_max[i];
  }
  box->max = max_abs;

  //---------------------------
}
