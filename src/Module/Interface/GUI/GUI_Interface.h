#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include "../../../common.h"

class GUI_module;
class GUI_Lidar;
class GUI_Network;


class GUI_Interface
{
public:
  //Constructor / Destructor
  GUI_Interface(GUI_module* node_gui);
  ~GUI_Interface();

public:
  void design_Interface();
  void design_Lidar();
  void design_Network();
  
  inline GUI_Lidar* get_gui_lidarManager(){return gui_lidarManager;}

private:
  GUI_Lidar* gui_lidarManager;
  GUI_Network* gui_netManager;

  int item_width;
};

#endif
