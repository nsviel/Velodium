#ifndef GUI_NETWORK_H
#define GUI_NETWORK_H

#include "../../common.h"

class Node_gui;
class MQTT;
class HTTP;


class GUI_Network
{
public:
  //Constructor / Destructor
  GUI_Network(Node_gui* node_gui);
  ~GUI_Network();

public:
  void design_Network();

  //MQTT
  void mqtt_connection();
  void mqtt_parameter();

  //HTTP
  void http_connection();
  void http_parameter();

private:
  MQTT* mqttManager;
  HTTP* httpsManager;

  int item_width;
};

#endif
