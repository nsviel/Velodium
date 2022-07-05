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
Trajectory::~Trajectory(){}

void Trajectory::create(){
  this->trajectory = new Glyph();
  //---------------------------

  //Create glyph
  trajectory->name = "Trajectory";
  trajectory->draw_width = width;
  trajectory->visibility = false;
  trajectory->draw_type = "line";
  trajectory->permanent = true;
  trajectory->color_unique = color;

  //Add an empty begin point
  trajectory->location.push_back(vec3(0,0,0));
  trajectory->color.push_back(color);

  //---------------------------
}
void Trajectory::update(Cloud*cloud){
  //---------------------------

  //Clear old values
  trajectory->location.clear();
  trajectory->color.clear();
  trajectory->visibility = visibility;

  //Compute new trajectory values
  for(int j=1; j<cloud->subset.size(); j++){
    Subset* subset_m0 = *next(cloud->subset.begin(), j);
    Frame* frame_m0 = &subset_m0->frame;

    Subset* subset_m1 = *next(cloud->subset.begin(), j-1);
    Frame* frame_m1 = &subset_m1->frame;

    if(subset_m0->visibility && frame_m0->is_slamed){
      vec3 trans_m0 = eigen_to_glm_vec3(frame_m0->trans_b);
      vec3 trans_m1 = eigen_to_glm_vec3(frame_m1->trans_b);
      vec4 color = trajectory->color_unique;

      //Add begin point
      trajectory->location.push_back(trans_m0);
      trajectory->color.push_back(color);

      //Add end pose
      trajectory->location.push_back(trans_m1);
      trajectory->color.push_back(color);
    }

  }

  //---------------------------
}
