#ifndef PROTOCOL_MQTT_H
#define PROTOCOL_MQTT_H

#include "../struct_wallet.h"

#include "../../../../common.h"

class Network;
class Configuration;
class PAHO;


class MQTT
{
public:
  //Constructor / Destructor
  MQTT(Network* net);
  ~MQTT();

public:
  void update_configuration();

  inline PAHO* get_mqtt_sncf(){return mqtt_sncf;}

private:
  Configuration* configManager;
  Network* netManager;

  PAHO* mqtt_sncf;
  PAHO* mqtt_local;
};

#endif
