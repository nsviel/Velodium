#include "Localmap.h"

#include "../Normal.h"


//Constructor / destructor
Localmap::Localmap(){
  //---------------------------

  this->color = vec4(0.0f, 0.3f, 0.4f, 1.0f);
  this->visibility = true;

  //---------------------------
  this->create_localmap();
}
Localmap::~Localmap(){}

void Localmap::create_localmap(){
  this->localmap = new Glyph();
  //---------------------------

  //Create glyph
  localmap->name = "localmap";
  localmap->draw_size = 5;
  localmap->draw_type = "point";
  localmap->color_unique = color;
  localmap->visibility = visibility;

  //---------------------------
}
void Localmap::update_localmap(voxelMap* map){
  //---------------------------

  vector<vec3>& gly_xyz = localmap->location;
  vector<vec4>& gly_rgb = localmap->color;

  gly_xyz.clear();
  gly_rgb.clear();

  for(voxelMap_it it = map->begin(); it != map->end(); ++it){
    for(int i=0; i<it->second.size(); i++){
      gly_xyz.push_back(vec3(it->second[i](0), it->second[i](1), it->second[i](2)));
      gly_rgb.push_back(color);
    }
  }

  //---------------------------
}
