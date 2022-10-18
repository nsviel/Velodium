#ifndef GUI_NETWORK_H
#define GUI_NETWORK_H

#include "../../../common.h"

class Module_obstacle;
class MQTT;
class HTTP;


class GUI_Network
{
public:
  //Constructor / Destructor
  GUI_Network(Module_obstacle* module);
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
