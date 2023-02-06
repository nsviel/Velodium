#include "SLAM_glyph.h"
#include "SLAM_map.h"
#include "SLAM.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Glyph/Object.h"
#include "../../../Engine/Scene/Glyph/SLAM/Localmap.h"
#include "../../../Engine/Scene/Glyph/SLAM/Car.h"
#include "../../../Engine/Scene/Glyph/SLAM/Trajectory.h"
#include "../../../Engine/Scene/Glyph/Cloud/Normal.h"
#include "../../../Engine/Scene/Glyph/Scene/Matching.h"
#include "../../../Specific/fct_math.h"


//Constructor / Destructor
SLAM_glyph::SLAM_glyph(SLAM* slam){
  //---------------------------

  Node_engine* node_engine = slam->get_node_engine();

  this->slam_map = slam->get_slam_map();
  this->objectManager = node_engine->get_objectManager();

  Trajectory* trajObject = objectManager->get_object_trajectory();
  Localmap* mapObject = objectManager->get_object_localmap();
  Matching* matchObject = objectManager->get_object_matching();
  Car* carObject = objectManager->get_object_car();

  this->trajectory = trajObject->get_glyph();
  this->localmap = mapObject->get_localmap();
  this->localcloud = mapObject->get_localcloud();
  this->car = carObject->get_glyph();
  this->matching = matchObject->get_glyph();

  this->with_keypoint = false;
  this->with_neighbor = false;
  this->with_matching = false;
  this->with_trajectory = true;
  this->with_localmap = false;
  this->with_localcloud = false;
  this->with_car = false;

  //---------------------------
}
SLAM_glyph::~SLAM_glyph(){}

//Main function
void SLAM_glyph::update_glyph(Cloud* cloud, Subset* subset){
  Glyph* keypoint = &subset->glyphs["keypoint"];
  //---------------------------

  //Clear vectors
  keypoint->xyz.clear();
  keypoint->rgb.clear();
  keypoint->Nxyz.clear();

  //Update glyphs
  this->update_glyph_keypoint(subset);
  this->update_glyph_nn(subset);
  this->update_glyph_matching(subset);
  this->update_glyph_normal(subset);
  this->update_glyph_map();
  this->update_glyph_car(cloud);
  this->update_glyph_trajectory(cloud);
  this->update_visibility(subset);

  //---------------------------
  objectManager->update_object(keypoint);
}
void SLAM_glyph::update_visibility(Subset* subset){
  Glyph* keypoint = &subset->glyphs["keypoint"];
  //---------------------------

  keypoint->visibility = with_keypoint | with_neighbor;
  trajectory->visibility = with_trajectory;
  localmap->visibility = with_localmap;
  localcloud->visibility = with_localcloud;
  car->visibility = with_car;
  matching->visibility = with_matching;

  //---------------------------
}
void SLAM_glyph::reset_glyph(){
  //---------------------------

  objectManager->reset_scene_object();

  //---------------------------
}

//Subfunctions
void SLAM_glyph::update_glyph_keypoint(Subset* subset){
  if(with_keypoint){
    Glyph* keypoint = &subset->glyphs["keypoint"];
    vector<vec3>& xyz = keypoint->xyz;
    vector<vec4>& rgb = keypoint->rgb;
    Frame* frame = &subset->frame;
    //---------------------------

    for(int i=0; i<frame->xyz.size(); i++){
      xyz.push_back(vec3(frame->xyz[i](0), frame->xyz[i](1), frame->xyz[i](2)));
      rgb.push_back(vec4(frame->ts_n[i], 1 - frame->ts_n[i], 0, 1));
    }

    //---------------------------
  }
}
void SLAM_glyph::update_glyph_nn(Subset* subset){
  if(with_neighbor){
    Glyph* keypoint = &subset->glyphs["keypoint"];
    vector<vec3>& xyz = keypoint->xyz;
    vector<vec4>& rgb = keypoint->rgb;
    Frame* frame = &subset->frame;
    //---------------------------

    //Initial checks
    if(frame->xyz.size() != frame->nn.size()){
      return;
    }

    // Frame keypoint nearest neighbor location and normal
    for(int i=0; i<frame->xyz.size(); i++){
      if(fct_is_nan(frame->nn[i]) == false){
        xyz.push_back(vec3(frame->nn[i](0), frame->nn[i](1), frame->nn[i](2)));
        rgb.push_back(vec4(frame->ts_n[i], 1 - frame->ts_n[i], 1, 1));
      }else{
        xyz.push_back(vec3(frame->nn[i](0), frame->nn[i](1), frame->nn[i](2)));
        rgb.push_back(vec4(1, 1, 1, 1));
      }
    }

    //---------------------------
  }
}
void SLAM_glyph::update_glyph_matching(Subset* subset){
  if(with_matching){
    vector<vec3> xyz_matching;
    Frame* frame = &subset->frame;
    vec4 color = vec4(0.7f, 0.1f, 0.1f, 1.0f);
    //---------------------------

    //Initial checks
    if(frame->xyz.size() != frame->nn.size()){
      return;
    }

    // Frame keypoint nearest neighbor location and normal
    for(int i=0; i<frame->xyz.size(); i++){
      xyz_matching.push_back(vec3(frame->xyz[i](0), frame->xyz[i](1), frame->xyz[i](2)));
      xyz_matching.push_back(vec3(frame->nn[i](0), frame->nn[i](1), frame->nn[i](2)));
    }

    // Update matching glyph
    Matching* matchObject = objectManager->get_object_matching();
    matchObject->update_matching(xyz_matching);
    objectManager->update_object(matchObject->get_glyph());

    //---------------------------
  }
}
void SLAM_glyph::update_glyph_normal(Subset* subset){
  vector<vec3> xyz;
  vector<vec3> Nxyz;
  Frame* frame = &subset->frame;
  //---------------------------

  // Frame keypoint nearest neighbor location and normal
  for(int i=0; i<frame->nn.size(); i++){
    xyz.push_back(vec3(frame->nn[i](0), frame->nn[i](1), frame->nn[i](2)));
    if(fct_is_nan(frame->N_nn[i]) == false){
      Nxyz.push_back(vec3(frame->N_nn[i](0), frame->N_nn[i](1), frame->N_nn[i](2)));
    }else{
      Nxyz.push_back(vec3(0, 0, 0));
    }
  }

  Normal* normalObject = objectManager->get_object_normal();
  normalObject->update_normal_subset(subset, xyz, Nxyz);

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
void SLAM_glyph::update_glyph_car(Cloud* cloud){
  if(with_car){
    //---------------------------

    Car* carObject = objectManager->get_object_car();
    Glyph* car = carObject->get_glyph();
    carObject->update_car_location(cloud);
    objectManager->update_object(car);

    //---------------------------
  }
}
void SLAM_glyph::update_glyph_trajectory(Cloud* cloud){
  if(with_trajectory){
    //---------------------------

    Trajectory* trajObject = objectManager->get_object_trajectory();
    Glyph* traj = trajObject->get_glyph();
    trajObject->update(cloud);
    objectManager->update_object(traj);

    //---------------------------
  }
}
