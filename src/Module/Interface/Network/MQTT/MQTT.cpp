//Sample at https://github.com/eclipse/paho.mqtt.cpp/blob/master/src/samples/async_publish.cpp
#include "MQTT.h"

#include "PAHO.h"

#include "../Network.h"
#include "../../Interface_node.h"

#include "../../../../Engine/Engine_node.h"
#include "../../../../Engine/Scene/Configuration.h"


//Constructor / Destructor
MQTT::MQTT(Network* net){
  this->netManager = net;
  //---------------------------

  Interface_node* node_interface = netManager->get_node_interface();
  Engine_node* node_engine = node_interface->get_node_engine();

  this->configManager = node_engine->get_configManager();
  this->mqtt_sncf = new PAHO();
  this->mqtt_local = new PAHO();

  //---------------------------
  this->update_configuration();
}
MQTT::~MQTT(){}

//Connection functions
void MQTT::update_configuration(){
  //---------------------------

  //SNCF broker
  string sncf_topic = configManager->parse_json_s("network", "mqtt_sncf_topic");
  string sncf_client = configManager->parse_json_s("network", "mqtt_sncf_client");
  string sncf_dest = configManager->parse_json_s("network", "mqtt_sncf_dest");
  string sncf_ip = netManager->get_ip_from_dest(sncf_dest);
  int sncf_port = configManager->parse_json_i("network", "mqtt_sncf_port");

  mqtt_sncf->configure(sncf_topic, sncf_client, sncf_ip, sncf_port);

  //Local broker
  string local_topic = configManager->parse_json_s("network", "mqtt_local_topic");
  string local_client = configManager->parse_json_s("network", "mqtt_local_client");
  string local_dest = configManager->parse_json_s("network", "mqtt_local_dest");
  string local_ip = netManager->get_ip_from_dest(sncf_dest);
  int local_port = configManager->parse_json_i("network", "mqtt_local_port");

  mqtt_local->configure(local_topic, local_client, local_ip, local_port);

  //---------------------------
}
