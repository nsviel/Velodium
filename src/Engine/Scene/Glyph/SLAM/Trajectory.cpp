#include "Trajectory.h"

#include "../../../../Specific/fct_transtypage.h"


//Constructor / destructor
Trajectory::Trajectory(){
  //---------------------------

  this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  this->visibility = false;
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

  trajectory->name = "Trajectory";
  trajectory->draw_width = width;
  trajectory->visibility = false;
  trajectory->draw_type = "line";
  trajectory->permanent = true;
  trajectory->color_unique = color;

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

    if(subset_m0->visibility && subset_m1->visibility){
      vec3 root_m0 = subset_m0->root;
      vec3 root_m1 = subset_m1->root;
      vec4 color = trajectory->color_unique;

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
  trajectory->visibility = false;

  //---------------------------
}
