#include "Object.h"

#include "Glyphs.h"
#include "Configuration.h"

#include "Object/Grid.h"
#include "Object/Axis.h"
#include "Object/AABB.h"
#include "Object/OOBB.h"
#include "Object/Mark.h"
#include "Object/Normal.h"
#include "Object/Trajectory.h"
#include "Object/Car.h"

#include "../Engine_node.h"


//Constructor / Destructor
Object::Object(Engine_node* node){
  //---------------------------

  this->glyphManager = node->get_glyphManager();
  this->configManager = node->get_configManager();
  this->gridObject = new Grid();
  this->axisObject = new Axis();
  this->aabbObject = new AABB();
  this->normObject = new Normal();
  this->oobbObject = new OOBB();
  this->markObject = new Mark();
  this->trajObject = new Trajectory();
  this->carObject = new Car();

  //---------------------------
  this->init_scene_object();
}
Object::~Object(){}

//Main functions
void Object::init_scene_object(){
  //---------------------------

  Glyph* aabb = aabbObject->get_aabb();
  Glyph* grid = gridObject->get_grid();

  glyphManager->create_glyph_scene(markObject->get_selection_frame());
  glyphManager->create_glyph_scene(grid);
  glyphManager->create_glyph_scene(gridObject->get_grid_sub());
  glyphManager->create_glyph_scene(gridObject->get_grid_plane());
  glyphManager->create_glyph_scene(axisObject->get_axis_scene());
  glyphManager->create_glyph_scene(trajObject->get_glyph());
  glyphManager->create_glyph_scene(aabb);
  glyphManager->create_glyph_scene(carObject->get_glyph());

  aabb->visibility = configManager->parse_json_b("glyph", "aabb_visibility");
  grid->visibility = configManager->parse_json_b("glyph", "grid_visibility");

  //---------------------------
}
void Object::runtime_subset_object(Subset* subset){
  //---------------------------

  if(subset->visibility){
    //Subset axis
    Glyph* axis = &subset->axis;
    glyphManager->draw_glyph(axis);

    //Normal
    Glyph* normal = &subset->normal;
    glyphManager->draw_glyph(normal);
  }

  //---------------------------
}

void Object::update_dynamic(Cloud* cloud){
  //---------------------------

  //Trajectory
  trajObject->update(cloud);
  this->update_object(trajObject->get_glyph());

  //Car 
  Glyph* glyph = carObject->get_glyph();
  glyph->visibility = true;
  carObject->update(cloud);
  glyphManager->update_glyph_location(glyph);

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

  //---------------------------
}

void Object::reset_scene_object(){
  //---------------------------

  //Remove non permanent glyphs
  glyphManager->remove_temporary_glyph();

  //Invisibilize all cloud dependant glyphs
  this->reset_object(aabbObject->get_aabb());

  //Reset specific glyphs
  this->reset_object(trajObject->get_glyph());

  Glyph* glyph = carObject->get_glyph();
  glyph->visibility = false;

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

void Object::create_glyph_subset(Subset* subset){
  //---------------------------

  //Cloud axis stuff
  axisObject->create_axis_subset(subset);
  glyphManager->insert_into_gpu(&subset->axis);

  //Normal stuff
  normObject->create_normal_subset(subset);
  glyphManager->insert_into_gpu(&subset->normal);

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
