#ifndef GUI_MENUBAR_H
#define GUI_MENUBAR_H

#include "../../common.h"

class GUI_windows;
class GUI_option;
class GUI_leftPanel;
class GUI_Player;

class Scene;
class Engine;
class Extraction;
class Heatmap;
class Textures;
class Operation;
class config_module;


class GUI_menuBar
{
public:
  //Constructor / Destructor
  GUI_menuBar(Engine* eng, GUI_windows* win, GUI_option* opt, GUI_leftPanel* lpa, GUI_Player* ply);
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
  GUI_windows* gui_winManager;
  GUI_option* optionManager;
  GUI_leftPanel* gui_leftPanelManager;
  GUI_Player* playerManager;

  Scene* sceneManager;
  Engine* engineManager;
  Operation* opeManager;
  Extraction* extractionManager;
  Heatmap* heatmapManager;
  Textures* texManager;
  config_module* configManager;
};


#endif
