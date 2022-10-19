#include "Node_operation.h"

#include "Color/GUI/GUI_Color.h"
#include "Dynamic/GUI/GUI_Player.h"
#include "Dynamic/GUI/GUI_Online.h"
#include "Capture/Capture.h"
#include "Color/Color.h"
#include "Color/Heatmap.h"
#include "Function/Selection.h"
#include "Function/CoordTransform.h"
#include "Function/Extraction.h"
#include "Transformation/Filter.h"
#include "Transformation/Attribut.h"
#include "Optimization/Fitting.h"
#include "Dynamic/Online.h"
#include "Dynamic/Player.h"
#include "Dynamic/Saving.h"

#include "../Engine/Engine_node.h"


//Constructor / Destructor
Node_operation::Node_operation(Engine_node* node){
  this->node_engine = node;
  //---------------------------

  this->node_load = node->get_node_load();
  this->fittingManager = new Fitting(this);
  this->attribManager = new Attribut(this);
  this->heatmapManager = new Heatmap(this);
  this->colorManager = new Color(this);
  this->filterManager = new Filter(this);
  this->coordManager = new CoordTransform(this);
  this->selectionManager = new Selection(this);
  this->extractionManager = new Extraction(this);
  this->onlineManager = new Online(this);
  this->playerManager = new Player(this);
  this->savingManager = new Saving(this);
  this->captureManager = new Capture(this);

  this->gui_color = new GUI_Color(this);
  this->gui_online = new GUI_Online(this);
  this->gui_player = new GUI_Player(this);

  //---------------------------
}
Node_operation::~Node_operation(){}

void Node_operation::update(){
  //---------------------------

  colorManager->update_configuration();
  filterManager->update_configuration();
  onlineManager->update_configuration();
  playerManager->update_configuration();
  captureManager->update_configuration();

  //---------------------------
}
void Node_operation::runtime(){
  //---------------------------

  playerManager->runtime();
  captureManager->runtime();
  gui_player->runtime_player_mouse();

  //---------------------------
}
