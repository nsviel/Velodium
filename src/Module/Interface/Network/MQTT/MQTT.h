#ifndef PROTOCOL_MQTT_H
#define PROTOCOL_MQTT_H

#include "../struct_wallet.h"

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
  void update_configuration();
  
  //Connection functions
  void mqtt_connection();
  void mqtt_disconnect();

  //Subfunctions
  void mqtt_test_localhost();
  void mqtt_send_message(string message);
  void mqtt_build_address();
  void mqtt_check_deconnection();

  inline string* get_message(){return &client_message;}
  inline string* get_topic(){return &broker_topic;}
  inline string* get_client_ID(){return &client_ID;}
  inline string* get_selected_ip(){return &selected_ip;}
  inline string* get_selected_dest(){return &selected_dest;}
  inline int* get_selected_port(){return &selected_port;}
  inline bool get_is_connected(){return is_connected;}

private:
  mqtt::async_client* client;

  //Broker
  string broker_topic;
  string client_message;
  string client_ID;

  //Connection
  string selected_dest;
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
