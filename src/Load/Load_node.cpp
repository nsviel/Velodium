#include "Load_node.h"


//Constructor / destructor
Load_node::Load_node(Engine_node* node){
  this->node_engine = node;
  //---------------------------

  this->extractManager = new Extractor(this);
  this->saveManager = new Saver(this);
  this->loadManager = new Loader(this);
  this->pathManager = new Pather(this);

  //---------------------------
}
Load_node::~Load_node(){}

void Load_node::update(){
  //---------------------------

  pathManager->update_configuration();

  //---------------------------
}
