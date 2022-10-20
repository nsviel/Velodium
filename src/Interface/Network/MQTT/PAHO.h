#ifndef PAHO_MQTT_H
#define PAHO_MQTT_H

#include "../../../common.h"

#include "mqtt/async_client.h"
#include <chrono>


class PAHO
{
public:
  //Constructor / Destructor
  PAHO();
  ~PAHO();

public:
  //Connection functions
  void paho_connection();
  void paho_disconnect();

  //Action funcitons
  void paho_subscrib();
  void paho_publish(string message);

  //Subfunctions
  void configure(string topic, string client, string ip, int port);
  void paho_build_address();
  void paho_check_deconnection();
  void paho_test_localhost();

  inline string* get_message(){return &client_message;}
  inline string* get_topic(){return &broker_topic;}
  inline string* get_client_ID(){return &client_ID;}
  inline string* get_selected_ip(){return &selected_ip;}
  inline int* get_selected_port(){return &selected_port;}
  inline bool get_is_connected(){return is_connected;}

private:
  mqtt::async_client* client;

  //Broker
  string broker_topic;
  string client_message;
  string client_ID;

  //Connection
  string selected_ip;
  string selected_address;
  int selected_port;

  //Parameters
  int qos;
  bool is_connected;
  string persist_dir;
  chrono::seconds timeout;
};

#endif
