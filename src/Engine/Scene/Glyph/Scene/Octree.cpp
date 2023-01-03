#include "Octree.h"


//Constructor / destructor
Octree::Octree(){
  //---------------------------

  this->octree_color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
  this->nb_cell = 4;

  this->create_octree();

  //---------------------------
}
Octree::~Octree(){
  //---------------------------

  delete octree;

  //---------------------------
}

void Octree::create_octree(){
  octree = new Glyph();
  //---------------------------

  //Create glyph
  octree->name = "octree";
  octree->draw_width = 2;
  octree->visibility = true;
  octree->draw_type = "line";
  octree->permanent = true;
  octree->color_unique = octree_color;

  //Construct octree
  this->update_octree(nb_cell);

  //---------------------------
}
void Octree::update_octree(int value){
  vector<vec3>& XYZ = octree->location;
  vector<vec4>& RGB = octree->color;
  this->nb_cell = value;
  //---------------------------

  //Parameters
  XYZ.clear();
  RGB.clear();

  //Construct octree
  for(int i=-nb_cell; i<=nb_cell; i++){
    XYZ.push_back(vec3((float)i, -(float)nb_cell, 0));
    XYZ.push_back(vec3((float)i, (float)nb_cell, 0));

    XYZ.push_back(vec3(-(float)nb_cell, (float)i, 0));
    XYZ.push_back(vec3((float)nb_cell, (float)i, 0));

    for(int j=0; j<4; j++){
      RGB.push_back(octree_color);
    }
  }

  //---------------------------
}
