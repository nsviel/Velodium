#include "Object.h"
#include "Glyphs.h"

#include "Base/Glyph_source.h"
#include "Scene/Grid.h"
#include "Scene/Axis.h"
#include "Scene/Mark.h"
#include "Scene/Matching.h"

#include "../../Module/SLAM/Glyph/Object/Keypoint.h"
#include "../../Module/SLAM/Glyph/Object/Trajectory.h"
#include "../../Module/SLAM/Glyph/Object/Car.h"
#include "../../Module/SLAM/Glyph/Object/Localmap.h"

#include "Cloud/AABB.h"
#include "Cloud/Box.h"
#include "Cloud/OOBB.h"
#include "Cloud/Normal.h"
#include "Cloud/Tree.h"

#include "../Node_scene.h"
#include "../Data/Scene.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Core/Configuration.h"


//Constructor / Destructor
Object::Object(Node_scene* node){
  this->node_engine = node->get_node_engine();
  //---------------------------

  this->glyphManager = node->get_glyphManager();
  this->configManager = node_engine->get_configManager();

  this->vec_glyph_src.push_back(new Grid());
  this->vec_glyph_src.push_back(new Axis());
  this->vec_glyph_src.push_back(new AABB());
  this->vec_glyph_src.push_back(new Normal());
  this->vec_glyph_src.push_back(new OOBB());
  this->vec_glyph_src.push_back(new Mark());
  this->vec_glyph_src.push_back(new Box());
  this->vec_glyph_src.push_back(new Tree());
  this->vec_glyph_src.push_back(new Matching());

  //this->vec_glyph_src.push_back(new Trajectory());
  //this->vec_glyph_src.push_back(new Car());
  //this->vec_glyph_src.push_back(new Keypoint());
  //this->vec_glyph_src.push_back(new Localmap());

  this->trajObject = new Trajectory();
  this->carObject = new Car();
  this->keyObject = new Keypoint();
  this->mapObject = new Localmap();

  //---------------------------
  this->create_glyph_scene();
  this->update_configuration();
}
Object::~Object(){
  //---------------------------


  //---------------------------
}

//Creation function
void Object::create_glyph_scene(){
  //---------------------------

  Mark* mark_src = (Mark*)get_glyph_src_byName("mark");
  Grid* grid_src = (Grid*)get_glyph_src_byName("grid");
  Axis* axis_src = (Axis*)get_glyph_src_byName("axis");
  AABB* aabb_src = (AABB*)get_glyph_src_byName("aabb");
  Box* box_src = (Box*)get_glyph_src_byName("box");
  Matching* match_src = (Matching*)get_glyph_src_byName("matching");

  glyphManager->create_glyph_scene(mark_src->get_selection_frame());
  glyphManager->create_glyph_scene(grid_src->get_grid());
  glyphManager->create_glyph_scene(grid_src->get_grid_sub());
  glyphManager->create_glyph_scene(grid_src->get_grid_plane());
  glyphManager->create_glyph_scene(axis_src->get_axis_scene());
  glyphManager->create_glyph_scene(aabb_src->get_glyph());
  glyphManager->create_glyph_scene(box_src->get_glyph());
  glyphManager->create_glyph_scene(match_src->get_glyph());
  glyphManager->create_glyph_scene(trajObject->get_glyph());
  glyphManager->create_glyph_scene(carObject->get_glyph());
  glyphManager->create_glyph_scene(mapObject->get_localmap());
  glyphManager->create_glyph_scene(mapObject->get_localcloud());

  //---------------------------
}
void Object::create_glyph_subset(Subset* subset){
  //---------------------------

  //Cloud axis glyph
  Axis* axis_src = (Axis*)get_glyph_src_byName("axis");
  axis_src->create_axis_subset(subset);
  glyphManager->insert_into_gpu(&subset->glyphs["axis"]);

  //Normal glyph
  Normal* normal_src = (Normal*)get_glyph_src_byName("normal");
  normal_src->create_glyph(subset);
  glyphManager->insert_into_gpu(&subset->glyphs["normal"]);

  //Keypoint glyph
  keyObject->create_glyph(subset);
  glyphManager->insert_into_gpu(&subset->glyphs["keypoint"]);

  //Tree glyph
  Tree* tree_src = (Tree*)get_glyph_src_byName("tree");
  tree_src->create_glyph(subset);
  glyphManager->insert_into_gpu(&subset->glyphs["tree"]);

  //---------------------------
}
Glyph* Object::create_glyph_ostacle(){
  //---------------------------

  //Creat new OOBB object
  Glyph* oobb = get_glyph_by_name("oobb");
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
    if(subset->is_visible){
      //Subset axis
      Glyph* axis = &subset->glyphs["axis"];
      glyphManager->draw_glyph(axis);
    }
  }

  //---------------------------
}
void Object::runtime_glyph_subset_selected(Subset* subset){
  if(subset == nullptr) return;
  //---------------------------

  //If the subset is visible
  if(subset->is_visible){
    //Keypoint
    Glyph* keypoint = &subset->glyphs["keypoint"];
    if(keypoint->is_visible){
      glyphManager->draw_glyph(keypoint);
    }

    //Normal
    Glyph* normal = &subset->glyphs["normal"];
    if(normal->is_visible){
      glyphManager->draw_glyph(normal);
    }

    //Tree
    Glyph* tree = &subset->glyphs["tree"];
    if(tree->is_visible){
      glyphManager->draw_glyph(tree);
    }
  }

  //---------------------------
}
void Object::runtime_glyph_pred(Subset* subset){
  //---------------------------

  //Check for conditions
  if(subset == nullptr) return;
  /*
  //OOBB - prediction
  vector<Glyph>& oobb_pr = subset->detection.oobb;
  for(int i=0; i<oobb_pr.size(); i++){
    Glyph* oobb = &oobb_pr[i];
    glyphManager->draw_glyph(oobb);
  }

  //OOBB - ground thruth
  vector<Glyph>& oobb_gt = subset->detection_gt.oobb;
  for(int i=0; i<oobb_gt.size(); i++){
    Glyph* oobb = &oobb_gt[i];
    glyphManager->draw_glyph(oobb);
  }*/

  //---------------------------
}

//Update function
void Object::update_configuration(){
  //---------------------------

  Glyph* aabb = get_glyph_by_name("aabb");
  Glyph* grid = get_glyph_by_name("grid");
  Glyph* axis = get_glyph_by_name("axis");
  aabb->is_visible = configManager->parse_json_b("glyph", "aabb_visibility");
  grid->is_visible = configManager->parse_json_b("glyph", "grid_visibility");
  axis->is_visible = configManager->parse_json_b("glyph", "axis_visibility");

  Glyph* car = carObject->get_glyph();
  Glyph* traj = trajObject->get_glyph();
  car->is_visible = false;
  traj->is_visible = false;

  bool* car_visu = carObject->get_visibility();
  bool* traj_visu = trajObject->get_visibility();
  *car_visu = configManager->parse_json_b("glyph", "car_visibility");
  *traj_visu = configManager->parse_json_b("glyph", "car_visibility");

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
    Axis* axis_src = (Axis*)get_glyph_src_byName("axis");
    axis_src->update_axis_subset(subset);
    glyphManager->update_glyph_location(&subset->glyphs["axis"]);
  }

  //Subset normal
  Normal* normal_src = (Normal*)get_glyph_src_byName("normal");
  normal_src->update_normal_subset(subset);
  this->update_object(&subset->glyphs["normal"]);

  //Tree
  Tree* tree_src = (Tree*)get_glyph_src_byName("tree");
  tree_src->update_tree(subset);
  this->update_object(&subset->glyphs["tree"]);

  //---------------------------
}
void Object::update_glyph_cloud(Cloud* cloud){
  //---------------------------

  //Update cloud AABB
  AABB* aabb_src = (AABB*)get_glyph_src_byName("aabb");
  aabb_src->update_glyph(cloud);
  this->update_object(aabb_src->get_glyph());

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
  Glyph* aabb = get_glyph_by_name("aabb");
  aabb->xyz.clear();

  //Reset specific glyphs
  for(int i=0; i<vec_glyph_src.size(); i++){
    vec_glyph_src[i]->reset();
  }

  Glyph* box = get_glyph_by_name("box");
  box->is_visible = false;

  mapObject->clear();

  //---------------------------
}
void Object::reset_color_object(){
  //---------------------------

  Glyph* aabb = get_glyph_by_name("aabb");
  Glyph* grid = get_glyph_by_name("grid");

  glyphManager->update_glyph_color(aabb, vec4(1.0f, 1.0f, 1.0f, 1.0f));
  glyphManager->update_glyph_color(grid, vec4(0.5f, 0.5f, 0.5f, 1.0f));

  //---------------------------
}
void Object::reset_object(Glyph* glyph){
  //---------------------------

  if(glyph != nullptr){
    glyph->xyz.clear();
    glyph->rgb.clear();
    glyphManager->update_glyph_location(glyph);
  }

  //---------------------------
}

//Misc function
void Object::set_object_visibility(string name, bool val){
  Node_scene* node_scene = node_engine->get_node_scene();
  Scene* sceneManager = node_scene->get_sceneManager();
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    //Set normal glyph visibility
    if(name == "normal"){
      for(int i=0; i<cloud->nb_subset; i++){
        Subset* subset = *next(cloud->subset.begin(), i);
        Glyph* normal = &subset->glyphs["normal"];
        normal->is_visible = val;
      }
    }
    //Set keypoint glyph visibility
    else if(name == "keypoint"){
      for(int i=0; i<cloud->nb_subset; i++){
        Subset* subset = *next(cloud->subset.begin(), i);
        Glyph* keypoint = &subset->glyphs["keypoint"];
        keypoint->is_visible = val;
      }
    }
    //Set tree glyph visibility
    else if(name == "tree"){
      for(int i=0; i<cloud->nb_subset; i++){
        Subset* subset = *next(cloud->subset.begin(), i);
        Glyph* tree = &subset->glyphs["tree"];
        tree->is_visible = val;
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
  localmap->is_visible = value;

  Glyph* localcloud = mapObject->get_localcloud();
  localcloud->is_visible = value;

  //---------------------------
}
Glyph* Object::get_glyph_by_name(string name){
  //---------------------------

  list<Glyph*>* list_glyph = glyphManager->get_list_glyph();
  for(int i=0; i<list_glyph->size(); i++){
    Glyph* glyph = *next(list_glyph->begin(), i);

    if(glyph->name == name){
      return glyph;
    }
  }

  //---------------------------
  return nullptr;
}
Glyph_source* Object::get_glyph_src_byName(string name){
  //---------------------------

  for(int i=0; i<vec_glyph_src.size(); i++){
    if(name == vec_glyph_src[i]->get_name()){
      return vec_glyph_src[i];
    }
  }

  //---------------------------
  return nullptr;
}
