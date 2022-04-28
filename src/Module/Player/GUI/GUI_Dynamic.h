#ifndef GUI_DYNAMIC_H
#define GUI_DYNAMIC_H

#include "../../../common.h"

class Scene;
class GUI_module;
class Player_cloud;
class Heatmap;
class Filter;
class Offline;
class Online;
class GUI_Interface;
class GUI_Color;
class GUI_Interface;
class Followup;


class GUI_Dynamic
{
public:
  //Constructor / Destructor
  GUI_Dynamic(GUI_module* gui_module);
  ~GUI_Dynamic();

public:
  //Main function
  void design_dynamic();

  //Subfunctions
  void player_run();
  void player_button();
  void player_selection();
  void parameter_offline();
  void parameter_online();
  void runtime_player_mouse();

private:
  Followup* followManager;
  Scene* sceneManager;
  Heatmap* heatmapManager;
  Filter* filterManager;
  Offline* offlineManager;
  Online* onlineManager;
  GUI_Interface* gui_interface;
  GUI_Color* gui_color;

  int item_width;
};

#endif
