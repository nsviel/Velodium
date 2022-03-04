#ifndef PROTOCOL_MQTT_H
#define PROTOCOL_MQTT_H

#include "../../../../common.h"

#include "mqtt/async_client.h"
#include <chrono>


class MQTT
{
public:
  //Constructor / Destructor
  MQTT();
  ~MQTT();

public:
  //Monolitic functions
  void mqtt_test_localhost();

  //Separated functions
  void mqtt_connexion();
  void mqtt_send_message(string message);
  void mqtt_disconnect();

  inline string* get_message(){return &client_message;}
  inline string* get_topic(){return &broker_topic;}
  inline string* get_broker_address(){return &broker_address;}
  inline string* get_client_ID(){return &client_ID;}

private:
  mqtt::async_client* client;

  //Connection
  string broker_address;
  string broker_topic;
  string client_message;
  string client_ID;

  //Parameters
  int qos;
  string persist_dir;
  chrono::seconds timeout;
};

#endif
