#ifndef PROTOCOL_MQTT_H
#define PROTOCOL_MQTT_H

#include "../../../common.h"

class Module_obstacle;
class Configuration;
class PAHO;


class MQTT
{
public:
  //Constructor / Destructor
  MQTT(Module_obstacle* module);
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
