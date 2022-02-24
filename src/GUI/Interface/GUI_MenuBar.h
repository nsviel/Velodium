#ifndef GUI_MENUBAR_H
#define GUI_MENUBAR_H

#include "../../common.h"

class GUI_windows;
class GUI_option;
class GUI_leftPanel;
class GUI_Dynamic;
class GUI_node;
class GUI_Initialization;

class Scene;
class Extraction;
class Textures;
class Pather;


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
  void MenuBar_Operations();

private:
  Scene* sceneManager;
  Pather* pathManager;
  Extraction* extractionManager;
  Textures* texManager;

  GUI_node* node_gui;
  GUI_windows* gui_winManager;
  GUI_option* optionManager;
  GUI_leftPanel* gui_leftPanelManager;
  GUI_Dynamic* gui_dynamic;
  GUI_Initialization* gui_init;
};


#endif
