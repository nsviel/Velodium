#ifndef ALERT_MQTT_H
#define ALERT_MQTT_H

#include "../../../../common.h"

class MQTT;


class Alert
{
public:
  //Constructor / Destructor
  Alert(MQTT* mqtt);
  ~Alert();

public:
  void send_prediction_by_mqtt(Subset* subset);

private:
  MQTT* mqttManager;
};

#endif
