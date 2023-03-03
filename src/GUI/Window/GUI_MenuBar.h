#ifndef GUI_MENUBAR_H
#define GUI_MENUBAR_H

#include "../../common.h"

class GUI_Modal;
class GUI_option;
class GUI_leftPanel;
class GUI_Player;
class Node_gui;
class GUI_Initialization;

class Scene;
class Extraction;
class Pather;
class CoreGLengine;
class Graph;


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
  void MenuBar_menu();
  void MenuBar_menu_file();
  void MenuBar_menu_load();
  void MenuBar_menu_save();
  void MenuBar_icons();
  void MenuBar_appInfo();
  void MenuBar_subsetSelection();
  void MenuBar_Operations();

private:
  Scene* sceneManager;
  Pather* pathManager;
  Extraction* extractionManager;
  CoreGLengine* glManager;
  Graph* graphManager;

  Node_gui* node_gui;
  GUI_Modal* gui_window;
  GUI_option* optionManager;
  GUI_leftPanel* gui_leftPanel;
  GUI_Player* gui_player;
  GUI_Initialization* gui_init;
};


#endif
