#include "SLAM_glyph.h"
#include "SLAM_map.h"
#include "SLAM.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Glyph/Object.h"
#include "../../../Engine/Scene/Glyph/SLAM/Localmap.h"
#include "../../../Specific/fct_math.h"


//Constructor / Destructor
SLAM_glyph::SLAM_glyph(SLAM* slam){
  //---------------------------

  Node_engine* node_engine = slam->get_node_engine();

  this->slam_map = slam->get_slam_map();
  this->objectManager = node_engine->get_objectManager();

  //---------------------------
}
SLAM_glyph::~SLAM_glyph(){}

//Main function
void SLAM_glyph::update_glyph(Subset* subset){
  //---------------------------

  this->update_glyph_keypoint(subset);
  this->update_glyph_nn(subset);
  this->update_glyph_normal(subset);
  this->update_glyph_map();

  //---------------------------
  objectManager->update_object(&subset->keypoint);
}
void SLAM_glyph::reset_glyph(){
  //---------------------------

  objectManager->reset_scene_object();

  //---------------------------
}

//Subfunctions
void SLAM_glyph::update_glyph_keypoint(Subset* subset){
  vector<vec3>& xyz = subset->keypoint.xyz;
  vector<vec4>& rgb = subset->keypoint.rgb;
  Frame* frame = &subset->frame;
  //vec4 color = vec4(0, 0, 1, 1.0f);
  vec4 color = vec4(0.7f, 0.1f, 0.1f, 1.0f);
  //---------------------------

  //Clear vectors
  xyz.clear();
  rgb.clear();

  //Fill vectors
  for(int i=0; i<frame->xyz.size(); i++){
    xyz.push_back(vec3(frame->xyz[i](0), frame->xyz[i](1), frame->xyz[i](2)));
    rgb.push_back(vec4(frame->ts_n[i], 1 - frame->ts_n[i], color[2], 1));
    //rgb.push_back(color);
  }

  //---------------------------
}
void SLAM_glyph::update_glyph_nn(Subset* subset){
  vector<vec3>& xyz = subset->keypoint.xyz;
  vector<vec4>& rgb = subset->keypoint.rgb;
  Frame* frame = &subset->frame;
  vec4 color = vec4(0.7f, 0.1f, 0.1f, 1.0f);
  //---------------------------

  //Initial checks
  if(frame->xyz.size() != frame->nn.size()){
    return;
  }

  // Frame keypoint nearest neighbor location and normal
  for(int i=0; i<frame->xyz.size(); i++){
    if(isnan(frame->nn[i](0)) == false){
      xyz.push_back(vec3(frame->nn[i](0), frame->nn[i](1), frame->nn[i](2)));
      rgb.push_back(vec4(frame->ts_n[i], 1 - frame->ts_n[i], color[2], 1));
    }else{
      xyz.push_back(vec3(0, 0, 0));
      rgb.push_back(vec4(1, 1, 1, 1));
    }
  }

  //---------------------------
}
void SLAM_glyph::update_glyph_normal(Subset* subset){
  vector<vec3>& Nxy = subset->keypoint.Nxyz;
  Frame* frame = &subset->frame;
  //---------------------------

  //Clear vector
  Nxy.clear();

  // Frame keypoint nearest neighbor location and normal
  for(int i=0; i<frame->nn.size(); i++){
    if(fct_is_nan(frame->nn[i]) == false){
      Nxy.push_back(vec3(frame->N_nn[i](0), frame->N_nn[i](1), frame->N_nn[i](2)));
    }else{
      Nxy.push_back(vec3(0, 0, 0));
    }
  }

  //---------------------------
}
void SLAM_glyph::update_glyph_map(){
  //---------------------------

  Localmap* mapObject = objectManager->get_object_localmap();
  mapObject->update_localmap(slam_map->get_local_map());
  objectManager->update_object(mapObject->get_localmap());
  mapObject->update_localcloud(slam_map->get_local_cloud());
  objectManager->update_object(mapObject->get_localcloud());

  //---------------------------
}
