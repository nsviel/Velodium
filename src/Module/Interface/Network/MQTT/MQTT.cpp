//Sample at https://github.com/eclipse/paho.mqtt.cpp/blob/master/src/samples/async_publish.cpp
#include "MQTT.h"

#include "MQTT_callback.h"

#include "../Network.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <cstring>
#include <map>


//Constructor / Destructor
MQTT::MQTT(Network* netManager){
  //---------------------------

  //Connection
  this->client_message = "Hello world!";
  this->client_ID = "ai_module";
  this->broker_topic = "ai_obstacle";

  //Parameters
  this->qos = 1;
  this->persist_dir	= "./persist";
  this->timeout = std::chrono::seconds(2);
  this->client = nullptr;
  this->is_connected = false;

  //---------------------------
  this->mqtt_wallet();
}
MQTT::~MQTT(){}

//Test localHost
void MQTT::mqtt_wallet(){
  //---------------------------

  //Create IP adresse wallet
  this->wallet.wallet_dic = {
    { "localhost", "tcp://localhost:1883" },
    { "ordi_nathan", "tcp:10.201.20.106:1883" },
    { "ordi_louis", "tcp:10.201.20.110:1883" },
    { "server_mine", "tcp:10.201.224.13:1883" },
  };

  //Concatenate it into combo format
  this->wallet.wallet_name.clear();
  this->wallet.wallet_ip.clear();
  for (auto const& [key, val] : wallet.wallet_dic){
    this->wallet.wallet_name.push_back(key);
    this->wallet.wallet_ip.push_back(val);
  }

  this->wallet.selected_name = "localhost";
  this->wallet.selected_ip = "tcp://localhost:1883";

  //---------------------------
}
void MQTT::mqtt_test_localhost(){
  //---------------------------

  // Initialize connection parameters
  this->mqtt_connexion();

  // First use a message pointer.
  this->mqtt_send_message(client_message);

  // Disconnect
  this->mqtt_disconnect();

  //---------------------------
}
void MQTT::mqtt_send_message(string message){
  //Check for connection
  if(mqtt_connexion()){
    //---------------------------

    callback cb;
    client->set_callback(cb);

    // First use a message pointer.
    mqtt::message_ptr pubmsg = mqtt::make_message(broker_topic, message);
    pubmsg->set_qos(qos);
    client->publish(pubmsg)->wait_for(timeout);

    //---------------------------
    cout << "Sending message [OK]" << endl;
  }
}

//Connection functions
bool MQTT::mqtt_connexion(){
  //---------------------------

  if(client == nullptr){
    //Initialize connection parameters
    this->client = new mqtt::async_client(wallet.selected_ip, client_ID, persist_dir);

    try {
      callback cb;
      client->set_callback(cb);
      auto connect_option = mqtt::connect_options_builder().clean_session().finalize();

      //Serveur connection
      mqtt::token_ptr token = client->connect(connect_option);
      // Blocks the current thread until the action this token is associated with has completed
      token->wait();
    }
    catch (const mqtt::exception& exc) {
      std::cerr << exc.what() << std::endl;
      return false;
    }

    if(client->is_connected()){
      cout << "Connection server '" << wallet.selected_ip << "' [OK]" << endl;
      this->is_connected = true;
      return true;
    }
  }else{
    if(client->is_connected()){
      this->is_connected = true;
      return true;
    }
  }

  //---------------------------
  this->is_connected = false;
  return false;
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
  this->is_connected = false;
  cout << "Disconnecting... [OK]" << endl;
}
