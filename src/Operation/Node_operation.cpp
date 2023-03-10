#include "Node_operation.h"

#include "Color/Color.h"
#include "Color/Heatmap.h"

#include "Dynamic/Online.h"
#include "Dynamic/Player.h"
#include "Dynamic/Onthefly.h"
#include "Dynamic/Visibility.h"

#include "Cloud/Selection.h"
#include "Transformation/Coordinate.h"
#include "Cloud/Extraction.h"
#include "Cloud/Boxing.h"

#include "Transformation/Filter.h"
#include "Transformation/Attribut.h"
#include "Optimization/Fitting.h"

#include "../Engine/Node_engine.h"


//Constructor / Destructor
Node_operation::Node_operation(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->node_load = node->get_node_load();

  this->fittingManager = new Fitting();
  this->attribManager = new Attribut();
  this->heatmapManager = new Heatmap();
  this->colorManager = new Color(this);
  this->filterManager = new Filter(this);
  this->coordManager = new Coordinate(this);
  this->selectionManager = new Selection(this);
  this->extractionManager = new Extraction(this);
  this->flyManager = new Onthefly(this);
  this->visibilityManager = new Visibility(this);
  this->onlineManager = new Online(this);
  this->playerManager = new Player(this);
  this->boxingManager = new Boxing(this);

  //---------------------------
}
Node_operation::~Node_operation(){}

void Node_operation::update(){
  //---------------------------

  colorManager->update_configuration();
  filterManager->update_configuration();
  onlineManager->update_configuration();
  playerManager->update_configuration();

  //---------------------------
}
void Node_operation::runtime(){
  //---------------------------

  playerManager->runtime();

  //---------------------------
}
void Node_operation::reset(){
  //---------------------------

  flyManager->reset();

  //---------------------------
}
