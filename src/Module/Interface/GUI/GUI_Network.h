#ifndef GUI_NETWORK_H
#define GUI_NETWORK_H

#include "../../../common.h"

class GUI_module;
class Network;
class MQTT;
class SFTP;
class SSH;


class GUI_Network
{
public:
  //Constructor / Destructor
  GUI_Network(GUI_module* gui_module);
  ~GUI_Network();

public:
  void sftp_connection();
  void ssh_connection();
  void ssh_parameter();
  void mqtt_connection();
  void mqtt_parameter();

private:
  Network* netManager;
  MQTT* mqttManager;
  SFTP* sftpManager;
  SSH* sshManager;

  int item_width;
};

#endif
