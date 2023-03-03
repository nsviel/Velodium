#include "Trajectory.h"


//Constructor / destructor
Trajectory::Trajectory(){
  //---------------------------

  this->name = "trajectory";
  this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->is_visible = false;
  this->width = 2;

  //---------------------------
  this->create();
}
Trajectory::~Trajectory(){
  //---------------------------

  delete trajectory;

  //---------------------------
}

void Trajectory::create(){
  this->trajectory = new Glyph();
  //---------------------------

  trajectory->name = "trajectory";
  trajectory->draw_line_width = width;
  trajectory->is_visible = false;
  trajectory->draw_type_name = "line";
  trajectory->is_permanent = true;
  trajectory->unicolor = color;

  //---------------------------
}
void Trajectory::update(Cloud*cloud){
  //---------------------------

  trajectory->xyz.clear();
  trajectory->rgb.clear();

  //Compute new trajectory values
  for(int j=1; j<cloud->subset.size(); j++){
    Subset* subset_m0 = *next(cloud->subset.begin(), j);
    Subset* subset_m1 = *next(cloud->subset.begin(), j-1);

    if(subset_m0->is_visible && subset_m1->is_visible){
      vec3 root_m0 = subset_m0->root;
      vec3 root_m1 = subset_m1->root;
      vec4 color = trajectory->unicolor;

      //Add begin point
      trajectory->xyz.push_back(root_m0);
      trajectory->rgb.push_back(color);

      //Add end pose
      trajectory->xyz.push_back(root_m1);
      trajectory->rgb.push_back(color);
    }

  }

  //---------------------------
}
void Trajectory::reset(){
  //---------------------------

  trajectory->xyz.clear();
  trajectory->rgb.clear();
  trajectory->is_visible = false;

  //---------------------------
}
