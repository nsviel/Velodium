#include "Node_interface.h"

#include "Capture/Capture.h"
#include "IO/Recorder.h"
#include "IO/PCAP/PCAP_reader.h"
#include "Network/HTTP/HTTP.h"

#include "../Engine/Node_engine.h"
#include "../Engine/Core/Configuration.h"


//Constructor / Destructor
Node_interface::Node_interface(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->node_load = node->get_node_load();
  this->node_ope = node->get_node_ope();

  this->configManager = node->get_configManager();
  this->captureManager = new Capture(this);
  this->recordManager = new Recorder(this);
  this->httpsManager = new HTTP(this);
  this->pcapManager = new PCAP_reader();

  //---------------------------
}
Node_interface::~Node_interface(){}

void Node_interface::update(){
  //---------------------------

  captureManager->update_configuration();
  httpsManager->update_configuration();
  recordManager->update_configuration();

  //---------------------------
}
void Node_interface::runtime(){
  //---------------------------

  captureManager->runtime();

  //---------------------------
}
