#ifndef WARNING_H
#define WARNING_H

#include "../../../common.h"

class MQTT;


class Warning
{
public:
  //Constructor / Destructor
  Warning(MQTT* mqtt);
  ~Warning();

public:
  void send_warning(Subset* subset);
  string build_jsonFile(Subset* subset);

private:
  MQTT* mqttManager;
};

#endif
