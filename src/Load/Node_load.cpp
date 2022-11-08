#include "Node_load.h"


//Constructor / destructor
Node_load::Node_load(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->extractManager = new Extractor(this);
  this->saverManager = new Saver(this);
  this->loaderManager = new Loader(this);
  this->patherManager = new Pather(this);

  //---------------------------
}
Node_load::~Node_load(){}

void Node_load::update(){
  //---------------------------

  patherManager->update_configuration();

  //---------------------------
}
