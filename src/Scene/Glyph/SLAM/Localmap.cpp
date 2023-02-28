#include "Localmap.h"

#include "../Cloud/Normal.h"


//Constructor / destructor
Localmap::Localmap(){
  //---------------------------

  this->color = vec4(66.0f/255.0f, 135.0f/255.0f, 245.0f/255.0f, 1.0f);

  //---------------------------
  this->create_localmap();
  this->create_localcloud();
}
Localmap::~Localmap(){
  //---------------------------

  delete localmap;
  delete localcloud;

  //---------------------------
}

void Localmap::clear(){
  //---------------------------

  localmap->xyz.clear();
  localmap->rgb.clear();

  localcloud->xyz.clear();
  localcloud->rgb.clear();

  //---------------------------
}
void Localmap::create_localmap(){
  this->localmap = new Glyph();
  //---------------------------

  //Create glyph
  localmap->name = "localmap";
  localmap->draw_size = 1;
  localmap->draw_type = "point";
  localmap->color_unique = color;
  localmap->visibility = false;
  localmap->permanent = true;

  //---------------------------
}
void Localmap::create_localcloud(){
  this->localcloud = new Glyph();
  //---------------------------

  //Create glyph
  localcloud->name = "localcloud";
  localcloud->draw_size = 1;
  localcloud->draw_type = "point";
  localcloud->color_unique = color;
  localcloud->visibility = false;
  localcloud->permanent = true;

  //---------------------------
}
void Localmap::update_localmap(slamap* map){
  if(localmap->visibility){
    //---------------------------

    vector<vec3>& gly_xyz = localmap->xyz;
    vector<vec4>& gly_rgb = localmap->rgb;

    gly_xyz.clear();
    gly_rgb.clear();

    for(voxelMap_it it = map->map.begin(); it != map->map.end(); it++){
      vector<Eigen::Vector3d>& voxel_xyz = it.value();
      for(int i=0; i<voxel_xyz.size(); i++){
        Eigen::Vector3d& point = voxel_xyz[i];

        vec3 xyz = vec3(point(0), point(1), point(2));
        vec4 rgb = localmap->color_unique;

        gly_xyz.push_back(xyz);
        gly_rgb.push_back(rgb);
      }
    }

    //---------------------------
  }
}
void Localmap::update_localcloud(slamap* map){
  if(localcloud->visibility){
    //---------------------------

    vector<vec3>& gly_xyz = localcloud->xyz;
    vector<vec4>& gly_rgb = localcloud->rgb;

    gly_xyz.clear();
    gly_rgb.clear();

    for(cloudMap_it it = map->cloud.begin(); it != map->cloud.end(); it++){
      vector<Eigen::Vector4d>& voxel_xyz = it.value();
      for(int i=0; i<voxel_xyz.size(); i++){
        Eigen::Vector4d& point = voxel_xyz[i];

        vec3 xyz = vec3(point(0), point(1), point(2));
        vec4 rgb = localcloud->color_unique;

        gly_xyz.push_back(xyz);
        gly_rgb.push_back(rgb);
      }
    }

    //---------------------------
  }
}
