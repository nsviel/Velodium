#ifndef NODE_GUI_H
#define NODE_GUI_H

#include "../common.h"

class Node_engine;
class Node_operation;
class Node_module;
class Node_interface;

class GUI;
class GUI_windows;
class GUI_control;
class GUI_option;
class GUI_menuBar;
class GUI_leftPanel;
class GUI_consol;
class GUI_fileManager;
class GUI_Initialization;
class GUI_Online;
class GUI_Player;
class GUI_Color;
class GUI_Network;
class GUI_Capture;
class GUI_Selection;


class Node_gui
{
public:
  //Constructor / Destructor
  Node_gui(Node_engine* engine);
  ~Node_gui();

public:
  //Main functions
  void init_objects();
  void reset();
  void exit();
  void loop();
  void loop_selection();

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_operation* get_node_ope(){return node_ope;}
  inline Node_module* get_node_module(){return node_module;}
  inline Node_interface* get_node_interface(){return node_interface;}

  inline GUI* get_guiManager(){return guiManager;}
  inline GUI_windows* get_gui_window(){return gui_window;}
  inline GUI_control* get_gui_control(){return gui_control;}
  inline GUI_option* get_gui_option(){return gui_option;}
  inline GUI_menuBar* get_gui_menuBar(){return gui_menuBar;}
  inline GUI_leftPanel* get_gui_leftPanel(){return gui_leftPanel;}
  inline GUI_consol* get_gui_consol(){return gui_consol;}
  inline GUI_fileManager* get_gui_fileManager(){return gui_fileManager;}
  inline GUI_Initialization* get_gui_initialization(){return gui_initialization;}
  inline GUI_Online* get_gui_online(){return gui_online;}
  inline GUI_Player* get_gui_player(){return gui_player;}
  inline GUI_Color* get_gui_color(){return gui_color;}
  inline GUI_Network* get_gui_network(){return gui_network;}
  inline GUI_Selection* get_gui_selection(){return gui_selection;}
  inline GUI_Capture* get_gui_capture(){return gui_capture;}

private:
  Node_engine* node_engine;
  Node_operation* node_ope;
  Node_module* node_module;
  Node_interface* node_interface;

  GUI* guiManager;
  GUI_windows* gui_window;
  GUI_control* gui_control;
  GUI_option* gui_option;
  GUI_menuBar* gui_menuBar;
  GUI_leftPanel* gui_leftPanel;
  GUI_consol* gui_consol;
  GUI_fileManager* gui_fileManager;
  GUI_Initialization* gui_initialization;
  GUI_Online* gui_online;
  GUI_Player* gui_player;
  GUI_Color* gui_color;
  GUI_Network* gui_network;
  GUI_Capture* gui_capture;
  GUI_Selection* gui_selection;
};

#endif
