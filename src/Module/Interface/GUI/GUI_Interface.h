#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include "../../../common.h"

class GUI_node;
class Network;
class MQTT;
class SFTP;
class SSH;


class GUI_Interface
{
public:
  //Constructor / Destructor
  GUI_Interface(GUI_node* node_gui);
  ~GUI_Interface();

public:
  void design_Network();

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
