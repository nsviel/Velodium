#include "Object.h"

#include "Glyphs.h"
#include "Configuration.h"

#include "Object/Scene/Grid.h"
#include "Object/Scene/Axis.h"
#include "Object/Scene/AABB.h"
#include "Object/Scene/Mark.h"

#include "Object/SLAM/Slam_keypoint.h"
#include "Object/SLAM/Trajectory.h"
#include "Object/SLAM/Car.h"
#include "Object/SLAM/Localmap.h"

#include "Object/Cloud/OOBB.h"
#include "Object/Cloud/Normal.h"

#include "../Node_engine.h"
#include "../Scene/Scene.h"


//Constructor / Destructor
Object::Object(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->glyphManager = node_engine->get_glyphManager();
  this->configManager = node_engine->get_configManager();
  this->gridObject = new Grid();
  this->axisObject = new Axis();
  this->aabbObject = new AABB();
  this->normObject = new Normal();
  this->oobbObject = new OOBB();
  this->markObject = new Mark();
  this->trajObject = new Trajectory();
  this->carObject = new Car();
  this->keyObject = new Slam_keypoint();
  this->mapObject = new Localmap();

  //---------------------------
  this->create_glyph_scene();
  this->update_configuration();
}
Object::~Object(){}

//Runtime functions
void Object::runtime_glyph_scene(){
  list<Glyph*>* list_glyph = glyphManager->get_list_glyph();
  //---------------------------

  for(int i=0;i<list_glyph->size();i++){
    Glyph* glyph = *next(list_glyph->begin(),i);
    glyphManager->draw_glyph(glyph);
  }

  //---------------------------
}
void Object::runtime_glyph_subset_all(Cloud* cloud){
  //---------------------------

  for(int j=0; j<cloud->subset.size(); j++){
    Subset* subset = *next(cloud->subset.begin(), j);

    //If the subset is visible
    if(subset->visibility){
      //Subset axis
      Glyph* axis = &subset->axis;
      glyphManager->draw_glyph(axis);
    }
  }

  //---------------------------
}
void Object::runtime_glyph_subset_selected(Subset* subset){
  //---------------------------

  //If the subset is visible
  if(subset->visibility){
    //Subset glyphs
    Glyph* keypoint = &subset->keypoint;
    Glyph* normal = &subset->normal;
    bool* visibility = keyObject->get_visibility();

    //Draw subset glyphs
    if(keypoint->visibility && *visibility){
      glyphManager->draw_glyph(keypoint);
      glyphManager->draw_glyph(normal);
    }
  }

  //---------------------------
}
void Object::runtime_glyph_pred(Subset* subset){
  //---------------------------

  //Check for conditions
  if(subset == nullptr) return;

  //OOBB - prediction
  vector<Glyph>& oobb_pr = subset->obstacle_pr.oobb;
  for(int i=0; i<oobb_pr.size(); i++){
    Glyph* oobb = &oobb_pr[i];
    glyphManager->draw_glyph(oobb);
  }

  //OOBB - ground thruth
  vector<Glyph>& oobb_gt = subset->obstacle_gt.oobb;
  for(int i=0; i<oobb_gt.size(); i++){
    Glyph* oobb = &oobb_gt[i];
    glyphManager->draw_glyph(oobb);
  }

  //---------------------------
}

//Update functions
void Object::update_configuration(){
  //---------------------------

  Glyph* aabb = aabbObject->get_aabb();
  Glyph* grid = gridObject->get_grid();
  aabb->visibility = configManager->parse_json_b("glyph", "aabb_visibility");
  grid->visibility = configManager->parse_json_b("glyph", "grid_visibility");

  Glyph* car = carObject->get_glyph();
  Glyph* traj = trajObject->get_glyph();
  car->visibility = false;
  traj->visibility = false;

  bool* car_visu = carObject->get_visibility();
  bool* traj_visu = trajObject->get_visibility();
  *car_visu = configManager->parse_json_b("glyph", "trajectory_visibility");
  *traj_visu = configManager->parse_json_b("glyph", "trajectory_visibility");

  //---------------------------
}
void Object::update_dynamic(Cloud* cloud){
  //---------------------------

  //Trajectory
  Glyph* traj = trajObject->get_glyph();
  trajObject->update(cloud);
  this->update_object(traj);

  //Car
  Glyph* car = carObject->get_glyph();
  carObject->update(cloud);
  glyphManager->update_glyph_location(car);

  //---------------------------
}
void Object::update_object(Glyph* glyph){
  //---------------------------

  glyphManager->update_glyph_location(glyph);
  glyphManager->update_glyph_color(glyph);

  //---------------------------
}
void Object::update_object(Glyph* glyph, vec4 color){
  //---------------------------

  glyphManager->update_glyph_location(glyph);
  glyphManager->update_glyph_color(glyph, color);

  //---------------------------
}
void Object::update_glyph_subset(Subset* subset){
  //---------------------------

  //Subset axis
  axisObject->update_axis_subset(subset);
  glyphManager->update_glyph_location(&subset->axis);

  //Subset keypoint
  keyObject->update_keypoint_location(subset);
  keyObject->update_keypoint_normal(subset);
  this->update_object(&subset->keypoint);

  //Subset normal
  normObject->update_normal_subset(subset);
  this->update_object(&subset->normal);

  //---------------------------
}
void Object::update_glyph_cloud(Cloud* cloud){
  //---------------------------

  //Update cloud AABB
  aabbObject->update_aabb(cloud);
  this->update_object(aabbObject->get_aabb());

  //Update cloud subset glyphs
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    this->update_glyph_subset(subset);
  }

  //---------------------------
}

//Reset functions
void Object::reset_scene_object(){
  //---------------------------

  //Remove non permanent glyphs
  glyphManager->remove_temporary_glyph();

  //Invisibilize all cloud dependant glyphs
  Glyph* aabb = aabbObject->get_aabb();
  aabb->location.clear();

  //Reset specific glyphs
  Glyph* traj = trajObject->get_glyph();
  traj->visibility = false;

  Glyph* car = carObject->get_glyph();
  car->visibility = false;

  //---------------------------
}
void Object::reset_color_object(){
  //---------------------------

  Glyph* aabb = aabbObject->get_aabb();
  Glyph* grid = gridObject->get_grid();

  glyphManager->update_glyph_color(aabb, vec4(1.0f, 1.0f, 1.0f, 1.0f));
  glyphManager->update_glyph_color(grid, vec4(0.5f, 0.5f, 0.5f, 1.0f));

  //---------------------------
}
void Object::reset_object(Glyph* glyph){
  //---------------------------

  if(glyph != nullptr){
    glyph->location.clear();
    glyph->color.clear();
    glyphManager->update_glyph_location(glyph);
  }

  //---------------------------
}

//Misc functions
void Object::set_object_visibility(string name, bool val){
  Scene* sceneManager = node_engine->get_sceneManager();
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    //Set normal glyph visibility
    if(name == "normal"){
      for(int i=0; i<cloud->nb_subset; i++){
        Subset* subset = *next(cloud->subset.begin(), i);
        Glyph* normal = &subset->normal;
        normal->visibility = val;
      }
    }
    //Set keypoint glyph visibility
    else if(name == "keypoint"){
      for(int i=0; i<cloud->nb_subset; i++){
        Subset* subset = *next(cloud->subset.begin(), i);
        Glyph* keypoint = &subset->keypoint;
        keypoint->visibility = val;
      }
    }
  }

  //---------------------------
}
void Object::set_slam_object(bool value){
  //---------------------------

  trajObject->set_visibility(value);
  carObject->set_visibility(value);
  keyObject->set_visibility(value);
  mapObject->set_visibility(value);

  //---------------------------
}
void Object::create_glyph_scene(){
  //---------------------------

  glyphManager->create_glyph_scene(markObject->get_selection_frame());
  glyphManager->create_glyph_scene(gridObject->get_grid());
  glyphManager->create_glyph_scene(gridObject->get_grid_sub());
  glyphManager->create_glyph_scene(gridObject->get_grid_plane());
  glyphManager->create_glyph_scene(axisObject->get_axis_scene());
  glyphManager->create_glyph_scene(trajObject->get_glyph());
  glyphManager->create_glyph_scene(aabbObject->get_aabb());
  glyphManager->create_glyph_scene(carObject->get_glyph());
  glyphManager->create_glyph_scene(mapObject->get_glyph());

  //---------------------------
}
void Object::create_glyph_subset(Subset* subset){
  //---------------------------

  //Cloud axis stuff
  axisObject->create_axis_subset(subset);
  glyphManager->insert_into_gpu(&subset->axis);

  //Normal stuff
  normObject->create_normal_subset(subset);
  glyphManager->insert_into_gpu(&subset->normal);

  //Keypoint stuff
  keyObject->create_keypoint(subset);
  glyphManager->insert_into_gpu(&subset->keypoint);

  //---------------------------
}
Glyph* Object::create_glyph_ostacle(){
  //---------------------------

  //Creat new OOBB object
  Glyph* oobb = oobbObject->create_oobb();
  glyphManager->insert_into_gpu(oobb);

  //---------------------------
  return oobb;
}
