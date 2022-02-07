#ifndef GUI_MENUBAR_H
#define GUI_MENUBAR_H

#include "../../common.h"

class GUI_windows;
class GUI_option;
class GUI_leftPanel;
class GUI_Player;
class GUI_node;

class Scene;
class Extraction;
class Heatmap;
class Textures;
class Operation;
class config_module;


class GUI_menuBar
{
public:
  //Constructor / Destructor
  GUI_menuBar(GUI_node* node_gui);
  ~GUI_menuBar();

public:
  //Main function
  void design_MenuBar();

  //Subfunctions
  void MenuBar_menus();
  void MenuBar_icons();
  void MenuBar_appInfo();
  void MenuBar_subsetSelection();
  void MenuBar_fastScene();
  void MenuBar_Operations();

private:
  Scene* sceneManager;
  Operation* opeManager;
  Extraction* extractionManager;
  Heatmap* heatmapManager;
  Textures* texManager;
  config_module* configManager;

  GUI_node* node_gui;
  GUI_windows* gui_winManager;
  GUI_option* optionManager;
  GUI_leftPanel* gui_leftPanelManager;
  GUI_Player* playerManager;
};


#endif
