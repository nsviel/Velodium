#include "Node_scene.h"

#include "Cloud/Scene.h"
#include "Glyph/Glyphs.h"
#include "Glyph/Object.h"

#include "../Engine/Node_engine.h"


//Constructor / Destructor
Node_scene::Node_scene(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->glyphManager = new Glyphs(this);
  this->objectManager = new Object(this);
  this->sceneManager = new Scene(this);

  //---------------------------
}
Node_scene::~Node_scene(){}

void Node_scene::update(){
  //---------------------------

  objectManager->update_configuration();

  //---------------------------
}
void Node_scene::reset(){
  //---------------------------

  objectManager->reset_scene_object();
  sceneManager->reset_cloud_all();

  //---------------------------
}
