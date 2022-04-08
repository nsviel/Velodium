#ifndef PROTOCOL_MQTT_H
#define PROTOCOL_MQTT_H

#include "../struct_wallet.h"

#include "../../../../common.h"

#include "mqtt/async_client.h"
#include <chrono>

class Network;


class MQTT
{
public:
  //Constructor / Destructor
  MQTT(Network* netManager);
  ~MQTT();

public:
  //Monolitic functions
  void mqtt_wallet();
  void mqtt_test_localhost();
  void mqtt_send_message(string message);

  //Separated functions
  bool mqtt_connexion();
  void mqtt_disconnect();

  inline Wallet* get_wallet(){return &wallet;}
  inline string* get_message(){return &client_message;}
  inline string* get_topic(){return &broker_topic;}
  inline string* get_client_ID(){return &client_ID;}
  inline bool get_is_connected(){return is_connected;}

private:
  mqtt::async_client* client;
  Wallet wallet;

  //Connection
  string broker_topic;
  string client_message;
  string client_ID;

  //Parameters
  int qos;
  bool is_connected;
  string persist_dir;
  chrono::seconds timeout;
};

#endif
