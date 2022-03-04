//Sample at https://github.com/eclipse/paho.mqtt.cpp/blob/master/src/samples/async_publish.cpp
#include "MQTT.h"

#include "MQTT_callback.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <cstring>


//Constructor / Destructor
MQTT::MQTT(){
  //---------------------------

  //Connection
  this->broker_address = "tcp://localhost:1883";
  this->broker_topic = "Obstacle";
  this->client_message = "Hello world!";
  this->client_ID = "AI_module";

  //Parameters
  this->qos = 1;
  this->persist_dir	= "./persist";
  this->timeout = std::chrono::seconds(2);
  this->client = nullptr;

  //---------------------------
}
MQTT::~MQTT(){}

void MQTT::mqtt_test_localhost(){
  //---------------------------

  this->broker_address  = "tcp://localhost:1883";
  this->client_ID = "Test_AI_module";

  // Initialize connection parameters
  this->mqtt_connexion();

  // First use a message pointer.
  this->mqtt_send_message(client_message);

  // Disconnect
  this->mqtt_disconnect();

  //---------------------------
}


void MQTT::mqtt_connexion(){
  if(client == nullptr){
    //---------------------------

    //Initialize connection parameters
    this->client = new mqtt::async_client(broker_address, client_ID, persist_dir);

    callback cb;
    client->set_callback(cb);
    auto connect_option = mqtt::connect_options_builder().clean_session().finalize();

    //Serveur connection
    mqtt::token_ptr token = client->connect(connect_option);
    token->wait();

    //---------------------------
    cout << "Connection server '" << broker_address << "' [OK]" << endl;
  }
}
void MQTT::mqtt_send_message(string message){
  //---------------------------

  //Check for connection
  this->mqtt_connexion();

  callback cb;
  client->set_callback(cb);

  // First use a message pointer.
  mqtt::message_ptr pubmsg = mqtt::make_message(broker_topic, message);
  pubmsg->set_qos(qos);
  client->publish(pubmsg)->wait_for(timeout);

  //---------------------------
  cout << "Sending message [OK]" << endl;
}
void MQTT::mqtt_disconnect(){
  //---------------------------

  callback cb;
  client->set_callback(cb);

  // Disconnect
  client->disconnect()->wait();
  delete client;
  client = nullptr;

  //---------------------------
  cout << "Disconnecting... [OK]" << endl;
}
