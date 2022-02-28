#ifndef NODE_GUI_H
#define NODE_GUI_H

#include "../common.h"

class Engine_node;
class Operation_node;
class Module_node;

class GUI;
class GUI_windows;
class GUI_control;
class GUI_option;
class GUI_menuBar;
class GUI_leftPanel;
class GUI_consol;
class GUI_module;
class GUI_fileManager;
class GUI_Initialization;


class GUI_node
{
public:
  //Constructor / Destructor
  GUI_node(Engine_node* engine);
  ~GUI_node();

public:
  //Main functions
  void init_objects();
  void reset();
  void update();
  void runtime();
  void exit();

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Operation_node* get_node_ope(){return node_ope;}
  inline Module_node* get_node_module(){return node_module;}

  inline GUI* get_guiManager(){return guiManager;}
  inline GUI_windows* get_gui_winManager(){return gui_winManager;}
  inline GUI_control* get_gui_controlManager(){return gui_controlManager;}
  inline GUI_option* get_gui_optionManager(){return gui_optionManager;}
  inline GUI_menuBar* get_gui_menuBarManager(){return gui_menuBarManager;}
  inline GUI_leftPanel* get_gui_leftPanelManager(){return gui_leftPanelManager;}
  inline GUI_consol* get_gui_consolManager(){return gui_consolManager;}
  inline GUI_module* get_gui_moduleManager(){return gui_moduleManager;}
  inline GUI_fileManager* get_gui_fileManager(){return gui_fileManager;}
  inline GUI_Initialization* get_gui_initManager(){return gui_initManager;}
private:
  Engine_node* node_engine;
  Operation_node* node_ope;
  Module_node* node_module;

  GUI* guiManager;
  GUI_windows* gui_winManager;
  GUI_control* gui_controlManager;
  GUI_option* gui_optionManager;
  GUI_menuBar* gui_menuBarManager;
  GUI_leftPanel* gui_leftPanelManager;
  GUI_consol* gui_consolManager;
  GUI_module* gui_moduleManager;
  GUI_fileManager* gui_fileManager;
  GUI_Initialization* gui_initManager;
};

#endif
