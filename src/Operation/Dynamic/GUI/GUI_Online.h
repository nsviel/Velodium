#ifndef GUI_ONLINE_H
#define GUI_ONLINE_H

#include "../../../common.h"

class Node_operation;
class Scene;
class Player_cloud;
class Heatmap;
class Filter;
class Offline;
class Capture;
class Online;
class GUI_Color;
class Followup;
class Saving;


class GUI_Online
{
public:
  //Constructor / Destructor
  GUI_Online(Node_operation* node_ope);
  ~GUI_Online();

public:
  //Main function
  void design_dynamic();



  //Main function
  void design_state();
  void design_time();

  //Subfunctions
  void state_configuration();
  void state_online();

private:
  Followup* followManager;
  Scene* sceneManager;
  Heatmap* heatmapManager;
  Filter* filterManager;
  Offline* offlineManager;
  Online* onlineManager;
  Capture* captureManager;
  GUI_Color* gui_color;
  Saving* savingManager;

  int item_width;
};

#endif
