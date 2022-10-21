#include "Node_interface.h"

#include "Capture/Capture.h"

#include "Network/HTTP/HTTP.h"
#include "Network/MQTT/MQTT.h"

#include "../Engine/Node_engine.h"
#include "../Engine/Scene/Configuration.h"


//Constructor / Destructor
Node_interface::Node_interface(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  this->node_load = node->get_node_load();
  this->node_ope = node->get_node_ope();

  this->configManager = node->get_configManager();
  this->captureManager = new Capture(this);
  this->mqttManager = new MQTT(this);
  this->httpsManager = new HTTP(this);

  //---------------------------
}
Node_interface::~Node_interface(){}

void Node_interface::update(){
  //---------------------------

  captureManager->update_configuration();
  mqttManager->update_configuration();
  httpsManager->update_configuration();

  //---------------------------
}
void Node_interface::runtime(){
  //---------------------------

  captureManager->runtime();

  //---------------------------
}
