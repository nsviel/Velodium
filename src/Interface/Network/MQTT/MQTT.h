#ifndef PROTOCOL_MQTT_H
#define PROTOCOL_MQTT_H

#include "../../../common.h"

class Node_interface;
class Configuration;
class PAHO;


class MQTT
{
public:
  //Constructor / Destructor
  MQTT(Node_interface* node_interface);
  ~MQTT();

public:
  void update_configuration();

  inline PAHO* get_mqtt_sncf(){return mqtt_sncf;}

private:
  Configuration* configManager;

  PAHO* mqtt_sncf;
  PAHO* mqtt_local;
};

#endif
