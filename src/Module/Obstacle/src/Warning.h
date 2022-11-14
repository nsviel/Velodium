#ifndef WARNING_H
#define WARNING_H

#include "../../../common.h"

class Module_obstacle;
//class MQTT;


class Warning
{
public:
  //Constructor / Destructor
  Warning(Module_obstacle* module);
  ~Warning();

public:
  void send_warning(Subset* subset);
  string build_jsonFile(Subset* subset);

private:
  //MQTT* mqttManager;
};

#endif
