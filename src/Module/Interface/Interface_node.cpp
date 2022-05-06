#include "Interface_node.h"

#include "Interface.h"
#include "Network/GPS.h"
#include "Network/HTTP/HTTP_server.h"
#include "Local/Saving.h"
#include "Local/Prediction.h"
#include "Network/Network.h"
#include "Local/Filemanager.h"

#include "LiDAR/Capture.h"

#include "../Module_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Interface_node::Interface_node(Module_node* node){
  //---------------------------

  this->node_module = node;
  this->node_engine = node_module->get_node_engine();

  this->fileManager = new Filemanager(this);
  this->saveManager = new Saving(this);
  this->predManager = new Prediction(this);
  this->captureManager = new Capture(this);
  this->gpsManager = new GPS(this);
  this->netManager = new Network();
  this->interfaceManager = new Interface(this);
  this->httpManager = new HTTP_server(this);

  //---------------------------
  this->init();
}
Interface_node::~Interface_node(){}

//Main functions
void Interface_node::init(){
  //---------------------------

  fileManager->check_directories();
  fileManager->clean_directories();

  //---------------------------
}
void Interface_node::runtime(){
  //---------------------------

  httpManager->loop();
  interfaceManager->runtime_loop();

  //---------------------------
}
