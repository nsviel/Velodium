//Sample at https://github.com/eclipse/paho.mqtt.cpp/blob/master/src/samples/async_publish.cpp
#include "MQTT.h"

#include "MQTT_callback.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <cstring>
#include <map>


//Constructor / Destructor
MQTT::MQTT(Wallet* wallet){
  //---------------------------

  //Connection
  this->client_message = "Hello world!";
  this->client_ID = "ai_module";
  this->broker_topic = "ai_obstacle";

  this->selected_dest = "localhost";
  this->selected_ip = "127.0.0.1";
  this->selected_port = 1883;
  this->selected_address = "tcp://" + selected_ip + ":" + to_string(selected_port);

  //Parameters
  this->qos = 1;
  this->persist_dir	= "./persist";
  this->timeout = std::chrono::seconds(2);
  this->client = nullptr;
  this->is_connected = false;

  //---------------------------
}
MQTT::~MQTT(){}

//Test localHost
void MQTT::mqtt_test_localhost(){
  //---------------------------

  // First use a message pointer.
  this->mqtt_send_message(client_message);

  // Disconnect
  this->mqtt_disconnect();

  //---------------------------
}
void MQTT::mqtt_send_message(string message){
  //---------------------------

  // Initialize connection parameters
  this->mqtt_connection();

  //Check for connection
  if(is_connected){
    callback cb;
    client->set_callback(cb);

    // First use a message pointer.
    mqtt::message_ptr pubmsg = mqtt::make_message(broker_topic, message);
    pubmsg->set_qos(qos);
    client->publish(pubmsg)->wait_for(timeout);

    //---------------------------
    cout << "Sending message [OK]" << endl;
  }

  //---------------------------
}
void MQTT::mqtt_build_address(){
  //---------------------------

  this->selected_address = "tcp://" + selected_ip + ":" + to_string(selected_port);
  this->mqtt_disconnect();

  //---------------------------
}

//Connection functions
void MQTT::mqtt_connection(){
  //---------------------------

  if(is_connected == false){
    //Initialize connection parameters
    //Problem ici parce que c'est pas terrible de refaire un nouveau pointer each time
    if(client != nullptr) delete client;
    this->client = new mqtt::async_client(selected_address, client_ID, persist_dir);

    try {
      callback cb;
      client->set_callback(cb);
      auto connect_option = mqtt::connect_options_builder()
        .automatic_reconnect(true)
        .clean_session()
        .finalize();

      //Serveur connection
      mqtt::token_ptr token = client->connect(connect_option);
      token->wait_for(std::chrono::seconds(2));

      //Display success
      if(client->is_connected()){
        string log = "Connection MQTT broker '" + selected_address + "'";
        console.AddLog("ok", log);
        this->is_connected = true;
      }else{
        string log = "Connection MQTT broker '" + selected_address + "' failed";
        console.AddLog("error", log);
      }
    }
    catch (const mqtt::exception& exc) {
      string log = "Connection MQTT broker '" + selected_address + "' failed";
      console.AddLog("error", log);
      this->is_connected = false;
    }
  }

  //---------------------------
}
void MQTT::mqtt_disconnect(){
  if(is_connected){
    //---------------------------

    callback cb;
    client->set_callback(cb);

    // Disconnect
    client->disconnect()->wait();
    delete client;
    client = nullptr;

    //---------------------------
    this->is_connected = false;
    cout << "Disconnecting... [OK]" << endl;
  }
}
