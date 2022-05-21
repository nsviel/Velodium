#include "Operation_node.h"


#include "Color/Color.h"
#include "Color/Heatmap.h"
#include "Functions/Selection.h"
#include "Functions/CoordTransform.h"
#include "Functions/Extraction.h"
#include "Transformation/Filter.h"
#include "Transformation/Attribut.h"
#include "Optimization/Fitting.h"

#include "../Engine/Engine_node.h"


//Constructor / Destructor
Operation_node::Operation_node(Engine_node* node){
  this->node_engine = node;
  //---------------------------

  this->fittingManager = new Fitting(this);
  this->attribManager = new Attribut(this);
  this->heatmapManager = new Heatmap(this);
  this->colorManager = new Color(this);
  this->filterManager = new Filter(this);
  this->coordManager = new CoordTransform(this);
  this->selectionManager = new Selection(this);
  this->extractionManager = new Extraction(this);

  //---------------------------
}
Operation_node::~Operation_node(){}

void Operation_node::update(){
  //---------------------------

  colorManager->update_configuration();
  filterManager->update_configuration();

  //---------------------------
}
