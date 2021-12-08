#ifndef MQTT_H
#define MQTT_H

#include "../../common.h"


class MQTT
{
public:
  //Constructor / Destructor
  MQTT();
  ~MQTT();

public:
  void mqtt_startConnexion();
  void mqtt_stopConnexion();

  inline bool* get_mqtt_connected(){return &mqtt_connected;}

private:
  bool mqtt_connected;
};

#endif
