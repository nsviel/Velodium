#include "Node_load.h"


//Constructor / destructor
Node_load::Node_load(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->extractManager = new Extractor();
  this->saverManager = new Saver();
  this->loaderManager = new Loader();
  this->patherManager = new Pather();

  //---------------------------
}
Node_load::~Node_load(){
  //---------------------------

  delete extractManager;
  delete saverManager;
  delete loaderManager;
  delete patherManager;

  //---------------------------
}

void Node_load::update(){
  //---------------------------

  patherManager->update_configuration();

  //---------------------------
}
