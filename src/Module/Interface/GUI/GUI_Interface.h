#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include "../../../common.h"

class GUI_module;
class GUI_Lidar;
class Network;
class MQTT;
class Alert;
class SFTP;
class SSH;
class Scene;


class GUI_Interface
{
public:
  //Constructor / Destructor
  GUI_Interface(GUI_module* node_gui);
  ~GUI_Interface();

public:
  void design_Interface();
  void design_Network();

  void ssh_connection();
  void ssh_parameter();
  void mqtt_connection();
  void mqtt_parameter();

  inline GUI_Lidar* get_gui_lidarManager(){return gui_lidarManager;}

private:
  Scene* sceneManager;
  GUI_Lidar* gui_lidarManager;
  Network* netManager;
  MQTT* mqttManager;
  Alert* alertManager;
  SFTP* sftpManager;
  SSH* sshManager;

  int item_width;
};

#endif
