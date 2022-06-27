//Sample at https://github.com/eclipse/paho.mqtt.cpp/blob/master/src/samples/async_publish.cpp
#include "PAHO.h"

#include "PAHO_callback.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <cstring>
#include <map>


//Constructor / Destructor
PAHO::PAHO(){
  //---------------------------

  this->client = nullptr;
  this->is_connected = false;

  //---------------------------
}
PAHO::~PAHO(){}

//Connection functions
void PAHO::paho_connection(){
  this->paho_check_deconnection();
  //---------------------------

  if(is_connected == false){
    //Initialize connection parameters
    this->client = new mqtt::async_client(selected_address, client_ID, persist_dir);

    try {

      auto connect_option = mqtt::connect_options_builder()
        .automatic_reconnect(false)
        .clean_session()
        .finalize();

      //Serveur connection
      mqtt::token_ptr token = client->connect(connect_option);
      token->wait_for(std::chrono::seconds(2));

      //Display success
      if(client->is_connected()){
        string log = "Connection PAHO broker '" + selected_address + "'";
        console.AddLog("ok", log);
        this->is_connected = true;
      }else{
        string log = "Connection PAHO broker '" + selected_address + "' failed";
        console.AddLog("error", log);
        this->is_connected = false;
        delete client;
      }
    }
    catch (const mqtt::exception& exc) {
      string log = "Connection PAHO broker '" + selected_address + "' failed";
      console.AddLog("error", log);
      this->is_connected = false;
      delete client;
    }
  }

  //---------------------------
}
void PAHO::paho_disconnect(){
  this->paho_check_deconnection();
  //---------------------------

  if(is_connected){
    // Disconnect
    client->disconnect()->wait();

    //Clear pointer
    delete client;
    client = nullptr;
    this->is_connected = false;

    //Display result
    cout << "Disconnecting... [OK]" << endl;
  }

  //---------------------------
}

//Action funcitons
void PAHO::paho_subscrib(){
  if(is_connected){
    //---------------------------

    //---------------------------
  }
}
void PAHO::paho_publish(string message){
  if(is_connected){
    //---------------------------

    //Callback
    callback cb;
    client->set_callback(cb);

    // First use a message pointer.
    mqtt::message_ptr pubmsg = mqtt::make_message(broker_topic, message);
    pubmsg->set_qos(qos);
    client->publish(pubmsg)->wait_for(timeout);

    //Result
    cout << "Sending message [OK]" << endl;

    //---------------------------
  }
}

//Subfunctions
void PAHO::configure(string topic, string client, string ip, int port){
  //---------------------------

  //Default
  this->client_message = "Hello world!";

  //Connection
  this->broker_topic = topic;
  this->client_ID = client;
  this->selected_ip = ip;
  this->selected_port = port;
  this->paho_build_address();

  //Parameters
  this->qos = 1;
  this->persist_dir	= "./persist";
  this->timeout = std::chrono::seconds(2);

  //---------------------------
}
void PAHO::paho_build_address(){
  //---------------------------

  this->selected_address = "tcp://" + selected_ip + ":" + to_string(selected_port);
  this->paho_disconnect();

  //---------------------------
}
void PAHO::paho_check_deconnection(){
  //---------------------------

  if(client != nullptr && is_connected == true){
    if(client->is_connected() == false){
      is_connected = false;
      delete client;
      client = nullptr;
    }
  }

  //---------------------------
}
void PAHO::paho_test_localhost(){
  string old_ip = selected_ip;
  this->selected_ip = "127.0.0.1";
  //---------------------------

  //Connect to localhost
  this->paho_connection();

  // Send message
  this->paho_publish("Hello world!");

  // Disconnect
  this->paho_disconnect();

  //---------------------------
  this->selected_ip = old_ip;
}
