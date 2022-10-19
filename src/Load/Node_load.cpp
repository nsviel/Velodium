#include "Node_load.h"


//Constructor / destructor
Node_load::Node_load(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->extractManager = new Extractor(this);
  this->saveManager = new Saver(this);
  this->loadManager = new Loader(this);
  this->pathManager = new Pather(this);

  //---------------------------
}
Node_load::~Node_load(){}

void Node_load::update(){
  //---------------------------

  pathManager->update_configuration();

  //---------------------------
}
