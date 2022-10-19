#ifndef GUI_MENUBAR_H
#define GUI_MENUBAR_H

#include "../../common.h"

class GUI_windows;
class GUI_option;
class GUI_leftPanel;
class GUI_Player;
class Node_gui;
class GUI_Initialization;

class Scene;
class Extraction;
class Textures;
class Pather;
class CoreGLengine;


class GUI_menuBar
{
public:
  //Constructor / Destructor
  GUI_menuBar(Node_gui* node_gui);
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
  CoreGLengine* glManager;

  Node_gui* node_gui;
  GUI_windows* gui_window;
  GUI_option* optionManager;
  GUI_leftPanel* gui_leftPanel;
  GUI_Player* gui_player;
  GUI_Initialization* gui_init;
};


#endif
