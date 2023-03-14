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
Object::Object(){
  //---------------------------

  this->glyphManager = new Glyphs();
  this->configManager = new Configuration();
  this->data = Data::get_instance();

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
  this->update_configuration();
}
void Object::create_glyph_object(Cloud* cloud){
  //---------------------------

  //Cloud axis glyph
  Axis* axis_src = (Axis*)get_glyph_src_byName("axis");
  Glyph* axis = axis_src->create_axis_subset(cloud);
  glyphManager->create_glyph_object(cloud, axis);

  //Normal glyph
  Normal* normal_src = (Normal*)get_glyph_src_byName("normal");
  Glyph* normal = normal_src->create_glyph(cloud);
  glyphManager->create_glyph_object(cloud, normal);

  //Keypoint glyph
  keyObject->create_glyph(cloud);
  glyphManager->insert_into_gpu(&cloud->glyphs["keypoint"]);

  //Tree glyph
  Tree* tree_src = (Tree*)get_glyph_src_byName("tree");
  Glyph* tree = tree_src->create_glyph(cloud);
  glyphManager->create_glyph_object(cloud, tree);

  //---------------------------
}
Glyph* Object::create_glyph_ostacle(){
  //---------------------------

  //Creat new OOBB object
  Glyph* oobb = get_glyph_scene_byName("oobb");
  glyphManager->insert_into_gpu(oobb);

  //---------------------------
  return oobb;
}

//Runtime function
void Object::draw_glyph_scene(){
  Collection* col_glyph = data->get_collection_byName("glyph", "glyph_scene");
  //---------------------------

  for(int i=0;i<col_glyph->list_obj.size();i++){
    Glyph* glyph = (Glyph*)*next(col_glyph->list_obj.begin(),i);
    glyphManager->draw_glyph(glyph);
  }

  //---------------------------
}
void Object::draw_glyph_object(){
  Collection* col_glyph = data->get_collection_byName("glyph", "glyph_object");
  //---------------------------

  for(int i=0;i<col_glyph->list_obj.size();i++){
    Glyph* glyph = (Glyph*)*next(col_glyph->list_obj.begin(),i);
    Object_* object = glyph->linked_object;
    if(object->is_visible){
      glyphManager->draw_glyph(glyph);
    }
  }

  //---------------------------
}

//Update function
void Object::update_configuration(){
  //---------------------------

  Glyph* aabb = get_glyph_scene_byName("aabb");
  Glyph* grid = get_glyph_scene_byName("grid");
  Glyph* axis = get_glyph_scene_byName("axis");
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
void Object::update_glyph(Glyph* glyph){
  //---------------------------

  glyphManager->update_glyph_location(glyph);
  glyphManager->update_glyph_color(glyph);

  //---------------------------
}

//Accesseurs
Glyph* Object::get_glyph_scene_byName(string name){
  Collection* col_glyph = data->get_collection_byName("glyph", "glyph_scene");
  //---------------------------

  for(int i=0; i<col_glyph->list_obj.size(); i++){
    Glyph* glyph = (Glyph*)*next(col_glyph->list_obj.begin(), i);

    if(glyph->name == name){
      return glyph;
    }
  }

  //---------------------------
  return nullptr;
}
Glyph* Object::get_glyph_object_byName(Cloud* cloud, string name){
  Collection* col_glyph = data->get_collection_byName("glyph", "glyph_object");
  //---------------------------

  for(int i=0; i<col_glyph->list_obj.size(); i++){
    Glyph* glyph = (Glyph*)*next(col_glyph->list_obj.begin(), i);

    if(glyph->name == name && glyph->linked_object->ID == cloud->ID){
      return glyph;
    }
  }

  //---------------------------
  return nullptr;
}





// A VIRER
void Object::update_glyph_cloud(Cloud* cloud){
  //---------------------------

  //Object axis
  Glyph* axis = get_glyph_object_byName(cloud, "axis");
  Axis* axis_src = (Axis*)get_glyph_src_byName("axis");
  axis_src->update_axis_subset(cloud, axis);
  glyphManager->update_glyph_location(axis);

  //Object normal
  Glyph* normal = get_glyph_object_byName(cloud, "normal");
  Normal* normal_src = (Normal*)get_glyph_src_byName("normal");
  normal_src->update_normal_cloud(cloud, normal);
  glyphManager->update_glyph_buffer(normal);

  //Tree
  Glyph* tree = get_glyph_object_byName(cloud, "tree");
  Tree* tree_src = (Tree*)get_glyph_src_byName("tree");
  tree_src->update_tree(cloud, tree);
  glyphManager->update_glyph_buffer(tree);

  //---------------------------
}
void Object::update_glyph_collection(Collection* collection){
  //---------------------------

  //Update cloud AABB
  Glyph* aabb = get_glyph_scene_byName("aabb");
  AABB* aabb_src = (AABB*)get_glyph_src_byName("aabb");
  aabb_src->update_glyph(collection, aabb);
  this->update_glyph(aabb);

  //Update cloud cloud glyphs
  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    this->update_glyph_cloud(cloud);
  }

  //---------------------------
}

//Reset function
void Object::reset_scene_object(){
  //---------------------------

  //Remove non permanent glyphs
  glyphManager->remove_temporary_glyph();

  //Invisibilize all cloud dependant glyphs
  Glyph* aabb = get_glyph_scene_byName("aabb");
  aabb->xyz.clear();

  //Reset specific glyphs
  for(int i=0; i<vec_glyph_src.size(); i++){
    vec_glyph_src[i]->reset();
  }

  Glyph* box = get_glyph_scene_byName("box");
  box->is_visible = false;

  mapObject->clear();

  //---------------------------
}
void Object::reset_color_object(){
  //---------------------------

  Glyph* aabb = get_glyph_scene_byName("aabb");
  Glyph* grid = get_glyph_scene_byName("grid");

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
  list<Collection*>* list_collection = data->get_list_col_object();
  //---------------------------

  for (int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);

    for(int i=0; i<collection->nb_obj; i++){
      Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
      Glyph* glyph = get_glyph_object_byName(cloud, name);
      glyph->is_visible = val;
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
