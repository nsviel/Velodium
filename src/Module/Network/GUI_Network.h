#ifndef GUI_NETWORK_H
#define GUI_NETWORK_H

class Network;
class MQTT;

#include "../../common.h"


class GUI_Network
{
public:
  //Constructor / Destructor
  GUI_Network();
  ~GUI_Network();

public:
  void design_Network();

  void connexion_ssh();
  void connexion_mqtt();

private:
  Network* netManager;
  MQTT* mqttManager;

};

#endif
