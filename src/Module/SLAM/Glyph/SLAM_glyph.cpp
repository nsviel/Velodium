#include "SLAM_glyph.h"

#include "Object/Localmap.h"
#include "Object/Car.h"
#include "Object/Trajectory.h"
#include "Object/Normal.h"
#include "Object/Matching.h"

#include "../src/SLAM_map.h"
#include "../src/SLAM.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Glyph/Object.h"
#include "../../../Specific/Function/fct_math.h"


//Constructor / Destructor
SLAM_glyph::SLAM_glyph(SLAM* slam){
  //---------------------------

  Node_engine* node_engine = slam->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->slam_map = slam->get_slam_map();
  this->objectManager = node_scene->get_objectManager();

  Trajectory* trajObject = objectManager->get_object_trajectory();
  Localmap* mapObject = objectManager->get_object_localmap();
  Matching* matchObject = objectManager->get_object_matching();
  Car* carObject = objectManager->get_object_car();

  this->list_glyph = new list<Glyph*>();
  list_glyph->push_back(trajObject->get_glyph());
  list_glyph->push_back(mapObject->get_localmap());
  list_glyph->push_back(mapObject->get_localcloud());
  list_glyph->push_back(carObject->get_glyph());
  list_glyph->push_back(matchObject->get_glyph());

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
void SLAM_glyph::update_glyph(Collection* collection, Cloud* cloud){
  Glyph* keypoint = &cloud->glyphs["keypoint"];
  //---------------------------

  //Clear vectors
  keypoint->xyz.clear();
  keypoint->rgb.clear();
  keypoint->Nxyz.clear();

  //Update glyphs
  this->update_glyph_keypoint(cloud);
  this->update_glyph_nn(cloud);
  this->update_glyph_matching(cloud);
  this->update_glyph_normal(cloud);
  this->update_glyph_map();
  this->update_glyph_car(collection);
  this->update_glyph_trajectory(collection);
  this->update_visibility(cloud);

  //---------------------------
  objectManager->update_glyph(keypoint);
}
void SLAM_glyph::update_visibility(Cloud* cloud){
  //---------------------------

  Glyph* keypoint = &cloud->glyphs["keypoint"];
  Glyph* trajectory = get_glyph_byName("trajectory");
  Glyph* localmap = get_glyph_byName("localmap");
  Glyph* localcloud = get_glyph_byName("localcloud");
  Glyph* car = get_glyph_byName("car");
  Glyph* matching = get_glyph_byName("matching");

  keypoint->is_visible = with_keypoint | with_neighbor;
  trajectory->is_visible = with_trajectory;
  localmap->is_visible = with_localmap;
  localcloud->is_visible = with_localcloud;
  car->is_visible = with_car;
  matching->is_visible = with_matching;

  //---------------------------
}
void SLAM_glyph::reset_glyph(){
  //---------------------------

  objectManager->reset_scene_object();

  //---------------------------
}

//Subfunctions
void SLAM_glyph::update_glyph_keypoint(Cloud* cloud){
  if(with_keypoint){
    Glyph* keypoint = &cloud->glyphs["keypoint"];
    vector<vec3>& xyz = keypoint->xyz;
    vector<vec4>& rgb = keypoint->rgb;
    Frame* frame = &cloud->frame;
    //---------------------------

    for(int i=0; i<frame->xyz.size(); i++){
      xyz.push_back(vec3(frame->xyz[i](0), frame->xyz[i](1), frame->xyz[i](2)));
      rgb.push_back(vec4(frame->ts_n[i], 1 - frame->ts_n[i], 0, 1));
    }

    //---------------------------
  }
}
void SLAM_glyph::update_glyph_nn(Cloud* cloud){
  if(with_neighbor){
    Glyph* keypoint = &cloud->glyphs["keypoint"];
    vector<vec3>& xyz = keypoint->xyz;
    vector<vec4>& rgb = keypoint->rgb;
    Frame* frame = &cloud->frame;
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
void SLAM_glyph::update_glyph_matching(Cloud* cloud){
  if(with_matching){
    vector<vec3> xyz_matching;
    Frame* frame = &cloud->frame;
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
    objectManager->update_glyph(matchObject->get_glyph());

    //---------------------------
  }
}
void SLAM_glyph::update_glyph_normal(Cloud* cloud){
  vector<vec3> xyz;
  vector<vec3> Nxyz;
  Frame* frame = &cloud->frame;
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
  normalObject->update_normal_cloud(cloud, xyz, Nxyz);

  //---------------------------
}
void SLAM_glyph::update_glyph_map(){
  //---------------------------

  Localmap* mapObject = objectManager->get_object_localmap();
  mapObject->update_localmap(slam_map->get_local_map());
  objectManager->update_glyph(mapObject->get_localmap());
  mapObject->update_localcloud(slam_map->get_local_cloud());
  objectManager->update_glyph(mapObject->get_localcloud());

  //---------------------------
}
void SLAM_glyph::update_glyph_car(Collection* collection){
  if(with_car){
    //---------------------------

    Car* carObject = objectManager->get_object_car();
    Glyph* car = carObject->get_glyph();
    carObject->update_glyph(collection);
    objectManager->update_glyph(car);

    //---------------------------
  }
}
void SLAM_glyph::update_glyph_trajectory(Collection* collection){
  if(with_trajectory){
    //---------------------------

    Trajectory* trajObject = objectManager->get_object_trajectory();
    Glyph* traj = trajObject->get_glyph();
    trajObject->update(collection);
    objectManager->update_glyph(traj);

    //---------------------------
  }
}

//Accesseurs
Glyph* SLAM_glyph::get_glyph_byName(string querry){
	//---------------------------

	//Search for corresponding shader and out it
	for(int i=0; i<list_glyph->size(); i++){
		Glyph* glyph = *next(list_glyph->begin(), i);
		if(glyph->name == querry){
			return glyph;
		}
	}

	//---------------------------
	return nullptr;
}
