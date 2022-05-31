#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include "../../../common.h"

class GUI_module;
class GUI_Lidar;
class GUI_Network;

class HTTPS;
class Interface;
class Saving;
class Prediction;
class GPS;


class GUI_Interface
{
public:
  //Constructor / Destructor
  GUI_Interface(GUI_module* gui_module);
  ~GUI_Interface();

public:
  void design_Interface();
  void design_Lidar();
  void design_Network();

  void parameter_dynamic();
  void state_watcher();
  void state_dynamic();

  inline GUI_Lidar* get_gui_lidarManager(){return gui_lidarManager;}

private:
  GUI_Lidar* gui_lidarManager;
  GUI_Network* gui_netManager;

  HTTPS* httpsManager;
  Interface* interfaceManager;
  Saving* saveManager;
  Prediction* predManager;
  GPS* gpsManager;

  int item_width;
};

#endif
