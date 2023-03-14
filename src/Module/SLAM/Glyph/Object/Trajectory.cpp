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
void Trajectory::update(Collection* collection){
  //---------------------------

  trajectory->xyz.clear();
  trajectory->rgb.clear();

  //Compute new trajectory values
  for(int j=1; j<collection->list_obj.size(); j++){
    Cloud* cloud_m0 = (Cloud*)*next(collection->list_obj.begin(), j);
    Cloud* cloud_m1 = (Cloud*)*next(collection->list_obj.begin(), j-1);

    if(cloud_m0->is_visible && cloud_m1->is_visible){
      vec3 root_m0 = cloud_m0->root;
      vec3 root_m1 = cloud_m1->root;
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
