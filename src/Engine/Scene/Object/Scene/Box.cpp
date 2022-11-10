#include "Box.h"


//Constructor / destructor
Box::Box(){
  //---------------------------

  this->color = vec4(0.0f, 0.0f, 0.8f, 0.05f);
  this->width = 1;

  //---------------------------
  this->create_box();
}
Box::~Box(){
  //---------------------------

  delete box;

  //---------------------------
}

//Main function
void Box::create_box(){
  box = new Glyph();
  //---------------------------

  //Create glyph
  box->name = "box";
  box->draw_width = width;
  box->visibility = false;
  box->draw_type = "triangle";
  box->permanent = true;
  box->color_unique = color;

  box->min = vec3(0,0,0);
  box->max = vec3(1,1,1);

  this->build_box_location();
  this->build_box_color();

  //---------------------------
}
void Box::update_box(){
  //---------------------------

  box->visibility = true;

  this->build_box_location();
  this->build_box_color();

  //---------------------------
}

//Subfunction
void Box::build_box_location(){
  vector<vec3>& xyz = box->location;
  vec3 min = box->min;
  vec3 max = box->max;
  //---------------------------

  //Clear old data
  xyz.clear();

  //Location
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

  //---------------------------
}
void Box::build_box_color(){
  vector<vec4>& rgb = box->color;
  //---------------------------

  rgb.clear();

  //Color
  for(int i=0; i<40; i++){
    rgb.push_back(box->color_unique);
  }

  //---------------------------
}
void Box::compute_box_MinMax(Subset* subset, vec3 min_perc, vec3 max_perc){
  //---------------------------

  //Get Z extremums
  vec3 min = subset->min;
  vec3 max = subset->max;
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

  say("----");
  sayVec3(min_abs);sayVec3(diff_max);

  //---------------------------
}
