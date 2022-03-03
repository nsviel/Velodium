#ifndef PROTOCOL_MQTT_H
#define PROTOCOL_MQTT_H

#include "../../../../common.h"

#include <chrono>


class MQTT
{
public:
  //Constructor / Destructor
  MQTT();
  ~MQTT();

public:
  void mqtt_test_localhost();
  void mqtt_sendMessages();

  inline string* get_message(){return &client_message;}
  inline string* get_topic(){return &client_topic;}
  inline string* get_server_address(){return &server_address;}
  inline string* get_client_ID(){return &client_ID;}

private:
  //Connection
  string server_address;
  string client_message;
  string client_topic;
  string client_ID;
  string client_json;

  //Parameters
  int qos;
  string persist_dir;
  chrono::seconds timeout;
};

#endif
