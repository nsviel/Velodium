#ifndef NODE_GUI_H
#define NODE_GUI_H

#include "../common.h"

class Engine_node;
class Node_operation;
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
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Module_node* get_node_module(){return node_module;}

  inline GUI* get_guiManager(){return guiManager;}
  inline GUI_windows* get_gui_window(){return gui_window;}
  inline GUI_control* get_gui_control(){return gui_control;}
  inline GUI_option* get_gui_option(){return gui_option;}
  inline GUI_menuBar* get_gui_menuBar(){return gui_menuBar;}
  inline GUI_leftPanel* get_gui_leftPanel(){return gui_leftPanel;}
  inline GUI_consol* get_gui_consol(){return gui_consol;}
  inline GUI_module* get_gui_module(){return gui_module;}
  inline GUI_fileManager* get_gui_fileManager(){return gui_fileManager;}
  inline GUI_Initialization* get_gui_initialization(){return gui_initialization;}

private:
  Engine_node* node_engine;
  Node_operation* node_ope;
  Module_node* node_module;

  GUI* guiManager;
  GUI_windows* gui_window;
  GUI_control* gui_control;
  GUI_option* gui_option;
  GUI_menuBar* gui_menuBar;
  GUI_leftPanel* gui_leftPanel;
  GUI_consol* gui_consol;
  GUI_module* gui_module;
  GUI_fileManager* gui_fileManager;
  GUI_Initialization* gui_initialization;
};

#endif
