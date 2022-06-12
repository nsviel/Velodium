#include "Localmap.h"

#include "../Normal.h"


//Constructor / destructor
Localmap::Localmap(){
  //---------------------------

  this->color = vec4(0.0f, 0.2f, 0.3f, 1.0f);
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
  localmap->draw_size = 1;
  localmap->draw_type = "point";
  localmap->color_unique = color;
  localmap->visibility = visibility;
  localmap->permanent = true;

  //---------------------------
}
void Localmap::update_localmap(voxelMap* map){
  //---------------------------

  vector<vec3>& gly_xyz = localmap->location;
  vector<vec4>& gly_rgb = localmap->color;

  gly_xyz.clear();
  gly_rgb.clear();

  if(visibility){
    for(voxelMap_it it = map->begin(); it != map->end(); it++){
      vector<Eigen::Vector3d>& voxel_xyz = it.value();
      for(int i=0; i<voxel_xyz.size(); i++){
        Eigen::Vector3d& point = voxel_xyz[i];
        vec3 xyz = vec3(point(0), point(1), point(2));
        gly_xyz.push_back(xyz);
        gly_rgb.push_back(color);
      }
    }
  }

  //---------------------------
}
