#ifndef GUI_NETWORK_H
#define GUI_NETWORK_H

class Network;
class MQTT;
class SFTP;
class SSH;

#include "../../../common.h"


class GUI_Network
{
public:
  //Constructor / Destructor
  GUI_Network();
  ~GUI_Network();

public:
  void design_Network();

  void ssh_connection();
  void mqtt_connection();
  void mqtt_parameter();

private:
  Network* netManager;
  MQTT* mqttManager;
  SFTP* sftpManager;
  SSH* sshManager;
};

#endif
