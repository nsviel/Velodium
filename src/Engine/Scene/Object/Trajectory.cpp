#include "Trajectory.h"


//Constructor / destructor
Trajectory::Trajectory(){
  //---------------------------

  this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->width = 2;

  //---------------------------
  this->create();
}
Trajectory::~Trajectory(){}

void Trajectory::create(){
  this->trajectory = new Glyph();
  //---------------------------

  //Create glyph
  trajectory->name = "Trajectory";
  trajectory->draw_width = width;
  trajectory->visibility = true;
  trajectory->draw_type = "line";
  trajectory->permanent = true;
  trajectory->color_unique = color;

  //Add an empty begin point
  trajectory->location.push_back(vec3(0,0,0));
  trajectory->color.push_back(color);

  //---------------------------
}
void Trajectory::update(Frame* frame){
  //---------------------------

  vec3 trans_abs = frame->trans_abs;
  vec4 color = trajectory->color_unique;

  trajectory->location.push_back(trans_abs);
  trajectory->color.push_back(color);

  trajectory->location.push_back(trans_abs);
  trajectory->color.push_back(color);

  //---------------------------
}
void Trajectory::reset(){
  //---------------------------

  //Clear previous data
  trajectory->location.clear();
  trajectory->color.clear();

  //Add an empty begin point
  trajectory->location.push_back(vec3(0,0,0));
  trajectory->color.push_back(color);

  //---------------------------
}