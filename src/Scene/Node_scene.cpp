#include "Node_scene.h"

#include "Data/Scene.h"
#include "Data/Graph.h"

#include "Glyph/Glyphs.h"
#include "Glyph/Object.h"

#include "../Engine/Node_engine.h"


//Constructor / Destructor
Node_scene::Node_scene(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->sceneManager = new Scene();
  this->glyphManager = new Glyphs();
  this->objectManager = new Object();
  this->graphManager = new Graph(this);

  //---------------------------
  this->init();
}
Node_scene::~Node_scene(){}

void Node_scene::init(){
  //---------------------------

  objectManager->create_glyph_scene();

  //---------------------------
}
void Node_scene::update(){
  //---------------------------

  objectManager->update_configuration();

  //---------------------------
}
void Node_scene::reset(){
  //---------------------------

  objectManager->reset_scene_object();
  sceneManager->reset_collection_all();

  //---------------------------
}
