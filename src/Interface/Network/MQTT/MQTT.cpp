//Sample at https://github.com/eclipse/paho.mqtt.cpp/blob/master/src/samples/async_publish.cpp
#include "MQTT.h"

#include "PAHO.h"

#include "../../Node_interface.h"

#include "../../../Engine/Scene/Configuration.h"


//Constructor / Destructor
MQTT::MQTT(Node_interface* node_interface){
  //---------------------------

  this->configManager = node_interface->get_configManager();
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
  string sncf_ip = "127.0.0.1";
  int sncf_port = configManager->parse_json_i("network", "mqtt_sncf_port");

  mqtt_sncf->configure(sncf_topic, sncf_client, sncf_ip, sncf_port);

  //Local broker
  string local_topic = configManager->parse_json_s("network", "mqtt_local_topic");
  string local_client = configManager->parse_json_s("network", "mqtt_local_client");
  string local_dest = configManager->parse_json_s("network", "mqtt_local_dest");
  string local_ip = "127.0.0.1";
  int local_port = configManager->parse_json_i("network", "mqtt_local_port");

  mqtt_local->configure(local_topic, local_client, local_ip, local_port);

  //---------------------------
}
