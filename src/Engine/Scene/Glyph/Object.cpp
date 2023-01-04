#include "Object.h"
#include "Glyphs.h"

#include "Scene/Grid.h"
#include "Scene/Axis.h"
#include "Scene/AABB.h"
#include "Scene/Mark.h"
#include "Scene/Box.h"
#include "Scene/Octree.h"

#include "SLAM/Slam_keypoint.h"
#include "SLAM/Trajectory.h"
#include "SLAM/Car.h"
#include "SLAM/Localmap.h"

#include "Cloud/OOBB.h"
#include "Cloud/Normal.h"

#include "../Configuration.h"
#include "../Scene.h"

#include "../../Node_engine.h"


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
  this->boxObject = new Box();
  this->octreeObject = new Octree();

  //---------------------------
  this->create_glyph_scene();
  this->update_configuration();
}
Object::~Object(){
  //---------------------------

  delete gridObject;
  delete axisObject;
  delete aabbObject;
  delete normObject;
  delete oobbObject;
  delete markObject;
  delete trajObject;
  delete carObject;
  delete keyObject;
  delete mapObject;
  delete boxObject;
  delete octreeObject;

  //---------------------------
}

//Creation function
void Object::create_glyph_scene(){
  //---------------------------

  glyphManager->create_glyph_scene(markObject->get_selection_frame());
  glyphManager->create_glyph_scene(gridObject->get_grid());
  glyphManager->create_glyph_scene(gridObject->get_grid_sub());
  glyphManager->create_glyph_scene(gridObject->get_grid_plane());
  glyphManager->create_glyph_scene(axisObject->get_axis_scene());
  glyphManager->create_glyph_scene(trajObject->get_glyph());
  glyphManager->create_glyph_scene(aabbObject->get_glyph());
  glyphManager->create_glyph_scene(carObject->get_glyph());
  glyphManager->create_glyph_scene(mapObject->get_localmap());
  glyphManager->create_glyph_scene(mapObject->get_localcloud());
  glyphManager->create_glyph_scene(boxObject->get_glyph());
  glyphManager->create_glyph_scene(octreeObject->get_glyph());

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

//Runtime function
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
  if(subset == nullptr) return;
  //---------------------------

  //If the subset is visible
  if(subset->visibility){
    //Keypoint
    Glyph* keypoint = &subset->keypoint;
    if(keypoint->visibility){
      glyphManager->draw_glyph(keypoint);
    }

    //Normal
    Glyph* normal = &subset->normal;
    if(normal->visibility){
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

//Update function
void Object::update_configuration(){
  //---------------------------

  Glyph* aabb = aabbObject->get_glyph();
  Glyph* grid = gridObject->get_grid();
  Glyph* axis = axisObject->get_axis_scene();
  aabb->visibility = configManager->parse_json_b("glyph", "aabb_visibility");
  grid->visibility = configManager->parse_json_b("glyph", "grid_visibility");
  axis->visibility = configManager->parse_json_b("glyph", "axis_visibility");

  Glyph* car = carObject->get_glyph();
  Glyph* traj = trajObject->get_glyph();
  car->visibility = false;
  traj->visibility = false;

  bool* car_visu = carObject->get_visibility();
  bool* traj_visu = trajObject->get_visibility();
  *car_visu = configManager->parse_json_b("glyph", "car_visibility");
  *traj_visu = configManager->parse_json_b("glyph", "car_visibility");

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
void Object::update_object(string obj, vec4 color){
  Glyph* glyph = get_glyph_by_name(obj);
  //---------------------------

  if(glyph != nullptr){
    glyphManager->update_glyph_location(glyph);
    glyphManager->update_glyph_color(glyph, color);
  }

  //---------------------------
}
void Object::update_glyph_subset(Subset* subset){
  //---------------------------

  //Subset axis
  if(configManager->parse_json_b("glyph", "axis_cloud_visibility")){
    axisObject->update_axis_subset(subset);
    glyphManager->update_glyph_location(&subset->axis);
  }

  //Subset keypoint
  keyObject->update_keypoint_location(subset);
  keyObject->update_keypoint_normal(subset);
  this->update_object(&subset->keypoint);

  //Subset normal
  normObject->update_normal_subset(subset);
  this->update_object(&subset->normal);

  //Octree
  Glyph* glyph = octreeObject->get_glyph();
  octreeObject->update_octree(subset);
  this->update_object(glyph);

  //---------------------------
}
void Object::update_glyph_cloud(Cloud* cloud){
  //---------------------------

  //Update cloud AABB
  aabbObject->update_aabb(cloud);
  this->update_object(aabbObject->get_glyph());

  //Update cloud subset glyphs
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    this->update_glyph_subset(subset);
  }

  //---------------------------
}

//Reset function
void Object::reset_scene_object(){
  //---------------------------

  //Remove non permanent glyphs
  glyphManager->remove_temporary_glyph();

  //Invisibilize all cloud dependant glyphs
  Glyph* aabb = aabbObject->get_glyph();
  aabb->location.clear();

  //Reset specific glyphs
  Glyph* traj = trajObject->get_glyph();
  traj->visibility = false;

  Glyph* car = carObject->get_glyph();
  car->visibility = false;

  Glyph* box = boxObject->get_glyph();
  box->visibility = false;

  mapObject->clear();

  //---------------------------
}
void Object::reset_color_object(){
  //---------------------------

  Glyph* aabb = aabbObject->get_glyph();
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

//Misc function
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

  Glyph* localmap = mapObject->get_localmap();
  localmap->visibility = value;

  Glyph* localcloud = mapObject->get_localcloud();
  localcloud->visibility = value;

  //---------------------------
}
Glyph* Object::get_glyph_by_name(string name){
  Glyph* glyph = nullptr;
  //---------------------------

  if(name == "aabb"){
    glyph = aabbObject->get_glyph();
  }else if(name == "selection"){
    glyph = markObject->get_selection_frame();
  }else if(name == "axis"){
    glyph = axisObject->get_axis_scene();
  }else if(name == "grid"){
    glyph = gridObject->get_grid();
  }

  //---------------------------
  return glyph;
}
