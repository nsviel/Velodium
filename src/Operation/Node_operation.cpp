#include "Node_operation.h"

#include "Color/GUI/GUI_Color.h"
#include "Color/Color.h"
#include "Color/Heatmap.h"

#include "Dynamic/Online.h"
#include "Dynamic/Player.h"
#include "Dynamic/Saving.h"
#include "Dynamic/GUI/GUI_Player.h"
#include "Dynamic/GUI/GUI_Online.h"

#include "Capture/Capture.h"
#include "Capture/GUI/GUI_Capture.h"

#include "Function/Selection.h"
#include "Function/CoordTransform.h"
#include "Function/Extraction.h"

#include "Transformation/Filter.h"
#include "Transformation/Attribut.h"
#include "Optimization/Fitting.h"

#include "../Engine/Node_engine.h"


//Constructor / Destructor
Node_operation::Node_operation(Node_engine* node){
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
  this->savingManager = new Saving(this);
  this->onlineManager = new Online(this);
  this->playerManager = new Player(this);
  this->captureManager = new Capture(this);

  this->gui_color = new GUI_Color(this);
  this->gui_player = new GUI_Player(this);
  this->gui_online = new GUI_Online(this);
  this->gui_capture = new GUI_Capture(this);

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
void Node_operation::draw(){
  //---------------------------

  gui_capture->design_Lidar();
  gui_online->design_dynamic();

  //---------------------------
}
